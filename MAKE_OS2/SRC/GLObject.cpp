
#include "GLObject.H"

#ifdef	DEBUG_MATRIX_STACK

// 行列スタックのデバッグモード
GLint DEBUG_modelViewMatrixStack = 0 ;
GLint DEBUG_projectionMatrixStack = 0 ;
GLint DEBUG_textureMatrixStack = 0 ;

void DEBUG_glPushMatrix()
{
	GLint mode ;
	glGetIntegerv(GL_MATRIX_MODE, &mode) ;

	switch (mode)
	{
	case GL_MODELVIEW:
		DEBUG_modelViewMatrixStack ++ ;
		break ;
	case GL_PROJECTION:
		DEBUG_projectionMatrixStack ++ ;
		break ;
	case GL_TEXTURE:
		DEBUG_textureMatrixStack ++ ;
		break ;
	}

#undef glPushMatrix
	glPushMatrix() ;
#define glPushMatrix	DEBUG_glPushMatrix
}

void DEBUG_glPopMatrix()
{
	GLint mode ;
	glGetIntegerv(GL_MATRIX_MODE, &mode) ;

	switch (mode)
	{
	case GL_MODELVIEW:
		DEBUG_modelViewMatrixStack -- ;
		if (DEBUG_modelViewMatrixStack < 0)
			MessageBox(NULL, "Modelview matrix underflow!", "glPopMatrix()", MB_OK | MB_ICONSTOP) ;
		break ;
	case GL_PROJECTION:
		DEBUG_projectionMatrixStack -- ;
		if (DEBUG_projectionMatrixStack < 0)
			MessageBox(NULL, "Projection matrix underflow!", "glPopMatrix()", MB_OK | MB_ICONSTOP) ;
		break ;
	case GL_TEXTURE:
		DEBUG_textureMatrixStack -- ;
		if (DEBUG_textureMatrixStack < 0)
			MessageBox(NULL, "Texture matrix underflow!", "glPopMatrix()", MB_OK | MB_ICONSTOP) ;
		break ;
	}

#undef glPopMatrix
	glPopMatrix() ;
#define glPopMatrix		DEBUG_glPopMatrix
}

#endif	// #ifdef DEBUG_MATRIX_STACK


// イメージフォーマットを RGB から RGBA に（RGBA フォーマットしか使えない環境用？）
// アルファ値のデフォルトは 1.0f
int RGBtoRGBA(TRUEIMAGE *image, GLfloat alpha)
{
	unsigned int x, y ;
	unsigned char *newImage ;

	if (!(newImage = (unsigned char *)malloc(image->width * image->height * 4))) return FAILURE ;

	if      (alpha < 0.0f) alpha = 0.0f ;
	else if (alpha > 0.0f) alpha = 1.0f ;

	for (y = 0 ; y < image->height ; y ++)
	{
		for (x = 0 ; x < image->width ; x ++)
		{
			unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3,	// 元イメージポインタ
						  *npt = newImage + y * image->width * 4 + x * 4 ;		// RGBAイメージポインタ

			npt[0] = ptr[0] ; npt[1] = ptr[1] ; npt[2] = ptr[2] ;

			npt[3] = (char)(alpha * 255.0f) ;	// アルファ値は 1.0f
		}
	}

	free(image->dataptr) ;
	image->dataptr = newImage ;

	return SUCCESS ;
}

// イメージコピー
TRUEIMAGE *CreateCopyImage(TRUEIMAGE *src)
{
	TRUEIMAGE *image ;
	image = (TRUEIMAGE *)malloc(sizeof(TRUEIMAGE)) ;
	if (!image) return NULL ;

	image->width  = src->width ;
	image->height = src->height ;

	image->dataptr = (unsigned char *)malloc((image->width + 4) * image->height * 3) ;
	if (!image->dataptr)
	{
		free(image) ;
		return NULL ;
	}

	// データ部のコピー
	memcpy(image->dataptr, src->dataptr, image->width * image->height * 3) ;

	return image ;
}

// 指定サイズの新たなイメージ生成（データは不明）
TRUEIMAGE *CreateNewImage(unsigned int w, unsigned int h)
{
	if (w == 0 && h == 0)
		return NULL ;

	TRUEIMAGE *image ;
	image = (TRUEIMAGE *)malloc(sizeof(TRUEIMAGE)) ;
	if (!image) return NULL ;

	image->width  = w ;
	image->height = h ;
	if (image->height == 0)
		image->height = w ;

	image->dataptr = (unsigned char *)malloc((image->width + 4) * image->height * 3) ;
	if (!image->dataptr)
	{
		free(image) ;
		return NULL ;
	}

	return image ;
}


// イメージクリア（デフォルトは白）
void ClearImage(TRUEIMAGE *image, float r, float g, float b, float radiusRatio,
				unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	unsigned int x, y ;

	if      (r < 0.0f) r = 0.0f ;
	else if (r > 1.0f) r = 1.0f ;

	if (g < 0.0f) g = r ;
	if (b < 0.0f) b = r ;

	if (r > 1.0f) r = 1.0f ;
	if (g > 1.0f) g = 1.0f ;
	if (b > 1.0f) b = 1.0f ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	if (radiusRatio == 0.0f)
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				ptr[0] = (unsigned char)(r * 255.0f) ;	// R
				ptr[1] = (unsigned char)(g * 255.0f) ;	// G
				ptr[2] = (unsigned char)(b * 255.0f) ;	// B
			}
		}
	}
	else
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				// 中心部からの距離
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				float ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;

				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				float change = r * 255.0f - ptr[0] ;
				ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

				change = g * 255.0f - ptr[0] ;
				ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

				change = b * 255.0f - ptr[0] ;
				ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
}


// イメージガンマ補正
void GammaCollection(TRUEIMAGE *image, float gamma, float radiusRatio,
					 unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (gamma == 1.0f || gamma == 0.0f) return ;
	else if (gamma == 0.0f) return ;
	gamma = 1.0f / gamma ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	if (radiusRatio == 0.0f)
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				ptr[0] = (unsigned char)(Pow(ptr[0] / 255.0f, gamma) * 255.0f) ;	// R 補正
				ptr[1] = (unsigned char)(Pow(ptr[1] / 255.0f, gamma) * 255.0f) ;	// G 補正
				ptr[2] = (unsigned char)(Pow(ptr[2] / 255.0f, gamma) * 255.0f) ;	// B 補正
			}
		}
	}
	else
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				// 中心部からの距離
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				float ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;

				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				float change = Pow(ptr[0] / 255.0f, gamma) * 255.0f - ptr[0] ;
				ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

				change = Pow(ptr[1] / 255.0f, gamma) * 255.0f - ptr[1] ;
				ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

				change = Pow(ptr[2] / 255.0f, gamma) * 255.0f - ptr[2] ;
				ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
}


// イメージ明るさ変化
void ChangeBrightness(TRUEIMAGE *image, float bright, float radiusRatio,
					  unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (bright == 1.0f) return ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	if (radiusRatio == 0.0f)
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ	

				ptr[0] = (unsigned char)(Clump(ptr[0] * bright, 0.0f, 255.0f)) ;	// R 補正
				ptr[1] = (unsigned char)(Clump(ptr[1] * bright, 0.0f, 255.0f)) ;	// G 補正
				ptr[2] = (unsigned char)(Clump(ptr[2] * bright, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
	else
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				// 中心部からの距離
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				float ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;

				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				float change = ptr[0] * bright - ptr[0] ;
				ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

				change = ptr[1] * bright - ptr[1] ;
				ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

				change = ptr[2] * bright - ptr[2] ;
				ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
}


// イメージ明るさ変化（ただしＲＧＢ別々に）
void ModulateRGB(TRUEIMAGE *image, float r, float g, float b, float radiusRatio,
				 unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (r == 1.0f && g == 1.0f && b == 1.0f ) return ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	if (radiusRatio == 0.0f)
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				ptr[0] = (unsigned char)(Clump(ptr[0] * r, 0.0f, 255.0f)) ;	// R 補正
				ptr[1] = (unsigned char)(Clump(ptr[1] * g, 0.0f, 255.0f)) ;	// G 補正
				ptr[2] = (unsigned char)(Clump(ptr[2] * b, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
	else
	{
		for (y = iy ; y < height ; y ++)
		{
			for (x = ix ; x < width ; x ++)
			{
				// 中心部からの距離
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				float ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;

				unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

				float change = ptr[0] * r - ptr[0] ;
				ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

				change = ptr[1] * g - ptr[1] ;
				ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

				change = ptr[2] * b - ptr[2] ;
				ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
			}
		}
	}
}


// イメージの彩度変更(０：グレースケール、１：オリジナル、２：２倍鮮やか)
void ChangeSaturation(TRUEIMAGE *image, float vivid, float radiusRatio,
					  unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (vivid == 1.0f) return ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	for (y = iy ; y < height ; y ++)
	{
		for (x = ix ; x < width ; x ++)
		{
			float ratio ;

			// 中心部からの距離
			if (radiusRatio)
			{
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;
			}
			else
				ratio = 1.0f ;

			unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

			// 彩度ゼロの場合の RGB 輝度（0〜255）
			float bright = ptr[0] * GRAY_RATIO_R + ptr[1] * GRAY_RATIO_G +  ptr[2] * GRAY_RATIO_B ;

			float change = (bright - ptr[0]) * (1 - vivid) ;
			ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

			change = (bright - ptr[1]) * (1 - vivid) ;
			ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

			change = (bright - ptr[2]) * (1 - vivid) ;
			ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
		}
	}
}


// イメージのコントラスト変更(０：グレー)
void ChangeContrast(TRUEIMAGE *image, float cont, float radiusRatio,
					unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (cont == 1.0f) return ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	for (y = iy ; y < height ; y ++)
	{
		for (x = ix ; x < width ; x ++)
		{
			float ratio ;

			// 中心部からの距離
			if (radiusRatio)
			{
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;
			}
			else
				ratio = 1.0f ;

			unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

			float change = (127 - (int)ptr[0]) * (1 - cont) ;
			ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

			change = (127 - (int)ptr[1]) * (1 - cont) ;
			ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

			change = (127 - (int)ptr[2]) * (1 - cont) ;
			ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
		}
	}
}


// イメージの明るさ補正
// 基本的にはガンマコレクションだが、
// R,G,B 全体の輝度から共通のガンマで補正するため、彩度が変化しない
// イメージの明るさ変更
void ChangeGammaBrightness(TRUEIMAGE *image, float gamma, float radiusRatio,
						   unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	// 変化なしなら即リターン
	if (gamma == 1.0f || gamma == 0.0f) return ;
	else if (gamma == 0.0f) return ;
	gamma = 1.0f / gamma ;

	unsigned int x, y ;

	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

	for (y = iy ; y < height ; y ++)
	{
		for (x = ix ; x < width ; x ++)
		{
			float ratio ;

			// 中心部からの距離
			if (radiusRatio)
			{
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;
			}
			else
				ratio = 1.0f ;

			unsigned char *ptr = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ

			// 全体輝度からのガンマ補正値
			float l = ((float)ptr[0] * GRAY_RATIO_R + (float)ptr[1] * GRAY_RATIO_G + (float)ptr[2] * GRAY_RATIO_B) / 255.0f ;
			if (l <= 0.0f || l >= 1.0f) continue ;
			float bright = Pow(l, gamma) ;

			// RGB による重み付けは不要？
//			float bright = Pow(((float)ptr[0] + (float)ptr[1] + (float)ptr[2]) / (255.0f * 3), gamma) ;

			float change = (float)ptr[0] * bright - (float)ptr[0] ;
			ptr[0] = (unsigned char)(Clump((float)ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

			change = (float)ptr[1] * bright - (float)ptr[1] ;
			ptr[1] = (unsigned char)(Clump((float)ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

			change = (float)ptr[2] * bright - (float)ptr[2] ;
			ptr[2] = (unsigned char)(Clump((float)ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
		}
	}
}


// image に image2 を加算（それぞれに対し重み付けが可能）
// image のみ、width, height 指定可能
void AddImage(TRUEIMAGE *image, TRUEIMAGE *image2, float weight, float weight2, float radiusRatio,
			  unsigned int ix, unsigned int iy, unsigned int width, unsigned int height)
{
	unsigned int x, y ;
	int allocFlag = FALSE ;
	width  += ix ;
	height += iy ;

	if (width  == ix || width  > image->width ) width  = image->width ;
	if (height == iy || height > image->height) height = image->height ;

//	if (width == 0 || height == 0)
//		return ;

	// image2 のサイズを image の該当部分に合わせる
	TRUEIMAGE image3 ;
	image3.width  = width  - ix ;
	image3.height = height - iy ;

	if (image3.width == image2->width && image3.height == image2->height)
		image3.dataptr = image2->dataptr ;
	else
	{
		allocFlag =TRUE ;

		image3.dataptr = (unsigned char *)malloc(sizeof(unsigned char) * (image3.width + 4) * image3.height * 3) ;
		if (!image3.dataptr)
		{
			return ;	// エラー処理（未定義）
		}

		int err =  gluScaleImage(GL_RGB, image2->width, image2->height,
								GL_UNSIGNED_BYTE, image2->dataptr,
								image3.width, image3.height,
								GL_UNSIGNED_BYTE, image3.dataptr) ;
		if (err)
		{
			freeimage(&image3) ;
			return ;	// エラー処理（未定義）
		}
	}

	// image が基準
	for (y = iy ; y < height ; y ++)
	{
		for (x = ix ; x < width ; x ++)
		{
			int x3 = x - ix, y3 = y - iy ;
			float ratio ;

			// 中心部からの距離
			if (radiusRatio)
			{
				float rx = (x - ix) / (float)(width  - ix) * 2.0f - 1.0f ;
				float ry = (y - iy) / (float)(height - iy) * 2.0f - 1.0f ;
				ratio = sqrt(rx * rx + ry * ry) ;

				if (ratio > 1.0f) continue ;

				// 距離による変更レート
				ratio = 1.0 - Pow(ratio, radiusRatio) ;
			}
			else
				ratio = 1.0f ;

			unsigned char *ptr  = image->dataptr + y * image->width * 3 + x * 3 ;	// R 要素のポインタ
			unsigned char *ptr3 = image3.dataptr + y3 * image3.width * 3 + x3 * 3 ;	// R 要素のポインタ

			float change = (ptr[0] * weight + ptr3[0] * weight2) - ptr[0] ;
			ptr[0] = (unsigned char)(Clump(ptr[0] + change * ratio, 0.0f, 255.0f)) ;	// R 補正

			change = (ptr[1] * weight + ptr3[1] * weight2) - ptr[1] ;
			ptr[1] = (unsigned char)(Clump(ptr[1] + change * ratio, 0.0f, 255.0f)) ;	// G 補正

			change = (ptr[2] * weight + ptr3[2] * weight2) - ptr[2] ;
			ptr[2] = (unsigned char)(Clump(ptr[2] + change * ratio, 0.0f, 255.0f)) ;	// B 補正
		}
	}

	if (allocFlag)
		freeimage(&image3) ;
}


// イメージの上下左右反転
void ReverseCoordinates(TRUEIMAGE *image, int mirrorFlag, int flipFlag)
{
	// 変化なしなら即リターン
	if (!mirrorFlag && !flipFlag) return ;

	// 作業用一時バッファ作成
	TRUEIMAGE *tmp ;
	tmp = (TRUEIMAGE *)malloc(sizeof(TRUEIMAGE)) ;
	if (!tmp) return ;
	tmp->dataptr = (unsigned char *)malloc(sizeof(unsigned char) * image->width * image->height * 3) ;
	if (!tmp->dataptr)
	{
		free(tmp) ;
		return ;
	}
	// データ部コピー
	memcpy(tmp->dataptr, image->dataptr, sizeof(unsigned char) * image->width * image->height * 3) ;

	// tmp を元に 座標反転しながらデータをコピー
	for (int y = 0 ; y < image->height ; y ++)
	{
		int sy ;
		if (flipFlag) sy = image->height - y - 1 ;
		else		  sy = y ;

		for (int x = 0 ; x < image->width ; x ++)
		{
			int sx ;
			if (mirrorFlag) sx = image->width - x - 1 ;
			else			sx = x ;

			unsigned char *srcptr =   tmp->dataptr + sy * image->width * 3 + sx * 3 ;	// R 要素のポインタ
			unsigned char *dstptr = image->dataptr +  y * image->width * 3 +  x * 3 ;	// R 要素のポインタ

			dstptr[0] = srcptr[0] ;
			dstptr[1] = srcptr[1] ;
			dstptr[2] = srcptr[2] ;
		}
	}

	freeimage(tmp) ;
}


// 視野ピラミッド作成（ジッタ情報含）
void Perspective(GLfloat scr_w, GLfloat scr_h, GLfloat n_clip, GLfloat f_clip,
				 int win_w, int win_h, GLfloat px, GLfloat py)
{
	if (win_w < 0 || win_h < 0)
	{
		GLint vp[4] ;
		glGetIntegerv(GL_VIEWPORT, vp) ;
		win_w = vp[2] ;
		win_h = vp[3] ;
	}

	px *= scr_w / win_w ;
	py *= scr_h / win_h ;

	scr_w *= 0.5f ;
	scr_h *= 0.5f ;

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	glFrustum(-scr_w + px, scr_w + px, -scr_h + py, scr_h + py, n_clip, f_clip) ;
}


// 色、素材セット
//
// r, g, b, a	素材の基本カラー
// metal		金属係数（高い程ディフューズが低く、スペキュラーが r, g, b の影響を受ける）
// specular		スペキュラー自体の明るさ
// shininess	ハイライト指数
// mirror		ミラーフラグ
// mirror_ratio	mirror == TRUE の時、この値が高い程金属係数によるディフューズの変化が少ない
// amb			!= NONE の場合、環境反射係数をディフューズとは別に与える（r, g, b 各要素 * amb)
// color_switch	
void SetMaterial(GLenum surface, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat metal, GLfloat specular, GLfloat shininess, GLint mirror, GLfloat mirror_ratio, GLfloat amb)
{
	// スペキュラーがゼロの場合
	if (specular == 0.0f)
		shininess = 0.0f ;

	GLfloat param[3][4], diffuse = 1.0f - metal, d_r, d_g, d_b ;
	if (mirror) diffuse = 1.0f - metal * (1.0f - mirror_ratio) ;

	// ディフューズ＆アンビエント
	param[0][0] = param[2][0] = d_r = r * diffuse ;
	param[0][1] = param[2][1] = d_g = g * diffuse ;
	param[0][2] = param[2][2] = d_b = b * diffuse ;
	param[0][3] = a ;

	// スペキュラー
	param[1][0] = (r + (1.0f - d_r) * (1.0f - metal)) * specular ;
	param[1][1] = (g + (1.0f - d_g) * (1.0f - metal)) * specular ;
	param[1][2] = (b + (1.0f - d_b) * (1.0f - metal)) * specular ;
	param[1][3] = 1.0f ;

//	if (shininess == 0.0f &&
//		(param[1][0] != 0.0f ||
//		 param[1][1] != 0.0f ||
//		 param[1][2] != 0.0f)
//		)
//		MessageBox(NULL, "AAAAAA", "GLU Error", MB_OK | MB_ICONSTOP) ;


	// amb がセットされている場合のアンビエント
	if (amb != NONE)
	{
		param[2][0] = r * amb ;
		param[2][1] = g * amb ;
		param[2][2] = b * amb ;
	}

	if (amb == NONE)
		glMaterialfv(surface, GL_AMBIENT_AND_DIFFUSE, param[0]) ;
	else
	{
		glMaterialfv(surface, GL_DIFFUSE, param[0]) ;
		glMaterialfv(surface, GL_AMBIENT, param[2]) ;
	}

	glMaterialfv(surface, GL_SPECULAR,  param[1]) ;
	glMaterialf (surface, GL_SHININESS, shininess) ;
}



void SetLightColor(GLenum light, GLfloat colorRGBA[4], GLfloat amb,
				   GLfloat specularFlag, GLfloat bright, GLfloat *modulate)
{
	GLfloat color[4] = { colorRGBA[0], colorRGBA[1], colorRGBA[2], 1.0f } ;	// colorRGBA[3] } ;
	GLfloat	lamb[4] ;	// = { color[0] * 0.1, color[1] * 0.1, color[2] * 0.1, 1.0f } ;
	static GLfloat	spec[4] = { 0.0f, 0.0f, 0.0f, 1.0f } ;

	if (amb != 0.0f)
	{
		lamb[0] = colorRGBA[0] * amb ;
		lamb[1] = colorRGBA[1] * amb ;
		lamb[2] = colorRGBA[2] * amb ;
	}
	else
	{
		lamb[0] = 0.0f ;
		lamb[1] = 0.0f ;
		lamb[2] = 0.0f ;
	}

	// bright が設定されている場合、ディフューズとスペキュラーの明るさ変更（環境光も変化）
	if (modulate)
	{
		color[0] *= bright * modulate[0] ;
		color[1] *= bright * modulate[1] ;
		color[2] *= bright * modulate[2] ;

		lamb[0] *= bright * modulate[0] ;
		lamb[1] *= bright * modulate[1] ;
		lamb[2] *= bright * modulate[2] ;
	}
	else
	{
		color[0] *= bright ;
		color[1] *= bright ;
		color[2] *= bright ;

		lamb[0] *= bright ;
		lamb[1] *= bright ;
		lamb[2] *= bright ;
	}

	glLightfv(light, GL_AMBIENT,  lamb) ;
	glLightfv(light, GL_DIFFUSE,  color) ;

	if (specularFlag)
	{
		GLfloat	specular[4] = { color[0] * specularFlag, color[1] * specularFlag, color[2] * specularFlag, 1.0f } ;
		glLightfv(light, GL_SPECULAR, specular) ;
	}
	else
		glLightfv(light, GL_SPECULAR, spec) ;
}


void SetDirectionalLight(GLenum light, GLfloat pos[4], GLfloat colorRGBA[4],
						 GLfloat amb, GLfloat specularFlag, GLfloat bright, GLfloat *modulate)
{
	GLfloat color[4] = { colorRGBA[0], colorRGBA[1], colorRGBA[2], 1.0f } ;	// colorRGBA[3] } ;
	GLfloat	lamb[4] = { color[0] * 0.1, color[1] * 0.1, color[2] * 0.1, 1.0f } ;
	static GLfloat	u_pos[4] ;
	static GLfloat	spec[4] = { 0.0f, 0.0f, 0.0f, 1.0f } ;

	if (amb != 0.0f)
	{
//		color[0] = colorRGBA[0] * (1.0f - amb) ;
//		color[1] = colorRGBA[1] * (1.0f - amb) ;
//		color[2] = colorRGBA[2] * (1.0f - amb) ;

		lamb[0] = colorRGBA[0] * amb ;
		lamb[1] = colorRGBA[1] * amb ;
		lamb[2] = colorRGBA[2] * amb ;
	}

	Vector3d u(pos) ;
	u.Unit() ;

	u_pos[0] = u.X() ;
	u_pos[1] = u.Y() ;
	u_pos[2] = u.Z() ;
	u_pos[3] = 0.0f ;
//	u_pos[3] = pos[3] ;

	// bright が設定されている場合、ディフューズとスペキュラーの明るさ変更（環境光も変化）
	if (modulate)
	{
		color[0] *= bright * modulate[0] ;
		color[1] *= bright * modulate[1] ;
		color[2] *= bright * modulate[2] ;

		lamb[0] *= bright * modulate[0] ;
		lamb[1] *= bright * modulate[1] ;
		lamb[2] *= bright * modulate[2] ;
	}
	else
	{
		color[0] *= bright ;
		color[1] *= bright ;
		color[2] *= bright ;

		lamb[0] *= bright ;
		lamb[1] *= bright ;
		lamb[2] *= bright ;
	}

	glLightfv(light, GL_POSITION, u_pos) ;
	glLightfv(light, GL_AMBIENT,  lamb) ;
	glLightfv(light, GL_DIFFUSE,  color) ;

//	if (specularFlag) glLightfv(light, GL_SPECULAR, color) ;
//	else              glLightfv(light, GL_SPECULAR, spec) ;

	if (specularFlag)
	{
		GLfloat	specular[4] = { color[0] * specularFlag, color[1] * specularFlag, color[2] * specularFlag, 1.0f } ;
		glLightfv(light, GL_SPECULAR, specular) ;
	}
	else
		glLightfv(light, GL_SPECULAR, spec) ;


	// 位置光源関連のパラメタは強制的にクリア

	// 位置減衰
	glLightf(light, GL_CONSTANT_ATTENUATION,  1.0f) ;
	glLightf(light, GL_LINEAR_ATTENUATION,    0.0f) ;
	glLightf(light, GL_QUADRATIC_ATTENUATION, 0.0f) ;

	// スポット光源パラメタ
	glLightfv(light, GL_SPOT_DIRECTION, u_pos) ;	// ダミー
	glLightf(light, GL_SPOT_CUTOFF, 180.0f) ;
	glLightf(light, GL_SPOT_EXPONENT, 0.0f) ;
}


void SetPositionalLight(GLenum light, GLfloat pos[4], GLfloat colorRGBA[4], GLfloat amb, GLfloat specularFlag, GLfloat bright,
						GLfloat const_attn, GLfloat linear_attn, GLfloat quad_attn,
						GLfloat *dir, GLfloat spot_cutoff, GLfloat spot_exp, GLfloat *modulate)
{
	GLfloat color[4] = { colorRGBA[0], colorRGBA[1], colorRGBA[2], 1.0f } ;	// colorRGBA[3] } ;
	GLfloat	lamb[4] = { color[0] * amb, color[1] * amb, color[2] * amb, 1.0f } ;
	static GLfloat	u_dir[4] = { 0.0f, 0.0f, -1.0f, 0.0f } ;
	static GLfloat	spec[4] = { 0.0f, 0.0f, 0.0f, 1.0f } ;

	if (amb != 0.0f)
	{
//		color[0] = colorRGBA[0] * (1.0f - amb) ;
//		color[1] = colorRGBA[1] * (1.0f - amb) ;
//		color[2] = colorRGBA[2] * (1.0f - amb) ;

		lamb[0] = colorRGBA[0] * amb ;
		lamb[1] = colorRGBA[0] * amb ;
		lamb[2] = colorRGBA[0] * amb ;
	}

	// bright が設定されている場合、ディフューズとスペキュラーの明るさ変更（環境光も変化）
	if (modulate)
	{
		color[0] *= bright * modulate[0] ;
		color[1] *= bright * modulate[1] ;
		color[2] *= bright * modulate[2] ;

		lamb[0] *= bright * modulate[0] ;
		lamb[1] *= bright * modulate[1] ;
		lamb[2] *= bright * modulate[2] ;
	}
	else
	{
		color[0] *= bright ;
		color[1] *= bright ;
		color[2] *= bright ;

		lamb[0] *= bright ;
		lamb[1] *= bright ;
		lamb[2] *= bright ;
	}

	if (dir)
	{
		Vector3d u(dir) ;
		u.Unit() ;

		u_dir[0] = u.X() ;
		u_dir[1] = u.Y() ;
		u_dir[2] = u.Z() ;
		u_dir[3] = dir[3] ;
	}

	glLightfv(light, GL_POSITION, pos) ;
	glLightfv(light, GL_AMBIENT,  lamb) ;
	glLightfv(light, GL_DIFFUSE,  color) ;

//	if (specularFlag) glLightfv(light, GL_SPECULAR, color) ;
//	else              glLightfv(light, GL_SPECULAR, spec) ;

	if (specularFlag)
	{
		GLfloat	specular[4] = { color[0] * specularFlag, color[1] * specularFlag, color[2] * specularFlag, 1.0f } ;
		glLightfv(light, GL_SPECULAR, specular) ;
	}
	else
		glLightfv(light, GL_SPECULAR, spec) ;


	// 位置減衰
	glLightf(light, GL_CONSTANT_ATTENUATION,  const_attn) ;
	glLightf(light, GL_LINEAR_ATTENUATION,    linear_attn) ;
	glLightf(light, GL_QUADRATIC_ATTENUATION, quad_attn) ;

	// スポット光源パラメタ
	glLightfv(light, GL_SPOT_DIRECTION, u_dir) ;
	glLightf(light, GL_SPOT_CUTOFF, spot_cutoff) ;
	glLightf(light, GL_SPOT_EXPONENT, spot_exp) ;
}


#ifndef NO_FONT
// 文字列描画
void DrawString(String& str, GLfloat x, GLfloat y, GLfloat z, void *font)
{
	glRasterPos3f(x, y, z) ;

	for (int i = 1 ; i <= str.Length() ; i ++)
		glutBitmapCharacter(font, str.Character(i)) ;
}

// 文字列描画
void DrawString(String& str, void *font)
{
	for (int i = 1 ; i <= str.Length() ; i ++)
		glutBitmapCharacter(font, str.Character(i)) ;
}
#else
void DrawString(String& str, GLfloat x, GLfloat y, GLfloat z, void *font) {}

// 文字列描画
void DrawString(String& str, void *font) {}
#endif

// サンプル・ジッタ生成（-0.5 〜 +0.5 の範囲）
Vector2d *CreateSampleJitter(int sample)
{
	Vector2d *jitter ;

	switch (sample)
	{
	case 0:
	case 1:
	case 2:
		sample = 2 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.25f, 0.75f) ;
		jitter[1](0.75f, 0.25f) ;
		break ;
	case 3:
		sample = 3 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.5033922635f, 0.8317967229f) ;
		jitter[1](0.7806016275f, 0.2504380877f) ;
		jitter[2](0.2261828938f, 0.4131553612f) ;
		break ;
	case 4:
		sample = 4 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.375f, 0.25f) ;
		jitter[1](0.125f, 0.75f) ;
		jitter[2](0.875f, 0.25f) ;
		jitter[3](0.625f, 0.75f) ;
		break ;
	case 5:
		sample = 5 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.5f, 0.5f) ;
		jitter[1](0.3f, 0.1f) ;
		jitter[2](0.7f, 0.9f) ;
		jitter[3](0.9f, 0.3f) ;
		jitter[4](0.1f, 0.7f) ;
		break ;
	case 6:
		sample = 6 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.4646464646f, 0.4646464646f) ;
		jitter[1](0.1313131313f, 0.7979797979f) ;
		jitter[2](0.5353535353f, 0.8686868686f) ;
		jitter[3](0.8686868686f, 0.5353535353f) ;
		jitter[4](0.7979797979f, 0.1313131313f) ;
		jitter[5](0.2020202020f, 0.2020202020f) ;
		break ;
	case 7:
	case 8:
		sample = 8 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.5625f, 0.4375f) ;
		jitter[1](0.0625f, 0.9375f) ;
		jitter[2](0.3125f, 0.6875f) ;
		jitter[3](0.6875f, 0.8125f) ;
		jitter[4](0.8125f, 0.1875f) ;
		jitter[5](0.9375f, 0.5625f) ;
		jitter[6](0.4375f, 0.0625f) ;
		jitter[7](0.1875f, 0.3125f) ;
		break ;
	case 9:
		sample = 9 ;
		jitter = new Vector2d[sample] ;
		jitter[0](0.5f         , 0.5f         ) ;
		jitter[1](0.1666666666f, 0.9444444444f) ;
		jitter[2](0.5f         , 0.1666666666f) ;
		jitter[3](0.5f         , 0.8333333333f) ;
		jitter[4](0.1666666666f, 0.2777777777f) ;
		jitter[5](0.8333333333f, 0.3888888888f) ;
		jitter[6](0.1666666666f, 0.6111111111f) ;
		jitter[7](0.8333333333f, 0.7222222222f) ;
		jitter[8](0.8333333333f, 0.0555555555f) ;
		break ;
	case 10:
	case 11:
	case 12:
		sample = 12 ;
		jitter = new Vector2d[sample] ;
		jitter[ 0](0.4166666666f, 0.625f) ;
		jitter[ 1](0.9166666666f, 0.875f) ;
		jitter[ 2](0.25f        , 0.375f) ;
		jitter[ 3](0.4166666666f, 0.125f) ;
		jitter[ 4](0.75f        , 0.125f) ;
		jitter[ 5](0.0833333333f, 0.125f) ;
		jitter[ 6](0.75f        , 0.625f) ;
		jitter[ 7](0.25f        , 0.875f) ;
		jitter[ 8](0.5833333333f, 0.375f) ;
		jitter[ 9](0.9166666666f, 0.375f) ;
		jitter[10](0.0833333333f, 0.625f) ;
		jitter[11](0.5833333333f, 0.875f) ;
		break ;
	case 13:
	case 14:
	case 15:
	case 16:
		sample = 16 ;
		jitter = new Vector2d[sample] ;
		jitter[ 0](0.375f, 0.4375f) ;
		jitter[ 1](0.625f, 0.0625f) ;
		jitter[ 2](0.875f, 0.1875f) ;
		jitter[ 3](0.125f, 0.0625f) ;
		jitter[ 4](0.375f, 0.6875f) ;
		jitter[ 5](0.875f, 0.4375f) ;
		jitter[ 6](0.625f, 0.5625f) ;
		jitter[ 7](0.375f, 0.9375f) ;
		jitter[ 8](0.625f, 0.3125f) ;
		jitter[ 9](0.125f, 0.5625f) ;
		jitter[10](0.125f, 0.8125f) ;
		jitter[11](0.375f, 0.1875f) ;
		jitter[12](0.875f, 0.9375f) ;
		jitter[13](0.875f, 0.6875f) ;
		jitter[14](0.125f, 0.3125f) ;
		jitter[15](0.625f, 0.8125f) ;
		break ;
	
	default:
		return NULL ;
	}

	for (int i = 0 ; i < sample ; i ++)
	{
		jitter[i].x -= 0.5f ;
		jitter[i].y -= 0.5f ;
	}

	return jitter ;
}

void DeleteSampleJitter(Vector2d *jitter)
{
	if (jitter) delete [] jitter ;
}

// 被写界深度用サンプル・ジッタ生成（-1.0 〜 +1.0 の範囲）
Vector2d *CreateSampleDepthOfFieldJitter(int sample)
{
	Vector2d *jitter ;

	switch (sample)
	{
	case 0:
	case 1:
	case 2:
		sample = 2 ;
		jitter = new Vector2d[sample] ;
		jitter[0](-1.0f, 0.0f) ;
		jitter[1]( 1.0f, 0.0f) ;
		break ;
	case 3:
		sample = 3 ;
		jitter = new Vector2d[sample] ;
		for (int i = 0 ; i < 3 ; i ++)
		{
			float rad = Rad(360.0f / 3 * i + 90.0f) ;
			jitter[i](cos(rad), sin(rad)) ;
		}
		break ;
	case 4:
		sample = 4 ;
		jitter = new Vector2d[sample] ;
		for (int i = 0 ; i < 4 ; i ++)
		{
			float rad = Rad(360.0f / 4 * i + 45.0f) ;
			jitter[i](cos(rad), sin(rad)) ;
		}
		break ;
	case 5:
		sample = 5 ;
		jitter = new Vector2d[sample] ;

		for (int i = 0 ; i < 4 ; i ++)
		{
			float rad = Rad(360.0f / 4 * i + 45.0f) ;
			jitter[i + 1](cos(rad), sin(rad)) ;
		}

		jitter[0](0.0f, 0.0f) ;
		break ;
	case 6:
	case 7:
		sample = 7 ;
		jitter = new Vector2d[sample] ;

		for (int i = 0 ; i < 6 ; i ++)
		{
			float rad = Rad(360.0f / 6 * i + 90.0f) ;
			jitter[i + 1](cos(rad), sin(rad)) ;
		}

		jitter[0](0.0f, 0.0f) ;
		break ;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		sample = 12 ;
		jitter = new Vector2d[sample] ;

		for (int i = 0 ; i < 8 ; i ++)
		{
			float rad = Rad(360.0f / 8 * i + 360.0 / 16.0f) ;
			jitter[i + 4](cos(rad), sin(rad)) ;
		}

		for (int i = 0 ; i < 4 ; i ++)
		{
			float rad = Rad(360.0f / 4 * i + 360.0 / 8.0f) ;
			jitter[i](cos(rad) * 0.48f, sin(rad) * 0.48f) ;
		}
		break ;
	
	default:
		return NULL ;
	}

	return jitter ;
}

Vector3d& Vector3d::Unit()
{
	GLfloat len = sqrt(X()*X() + Y()*Y() + Z()*Z()) ;
	if (len != 0.0f) *this /= len ;
	return *this ;
}

/*
const Vector3d Unit(Vector3d& vec)
{
	return (Vector3d(vec).Unit()) ;
}
*/
Vector3d Unit(const Vector3d& vec)
{
	return (Vector3d(vec).Unit()) ;
}

/*
const Vector3d UnitNormal(Vector3d& v1, Vector3d& v2)
{
	return Unit(v1 | v2) ;
}
const Vector3d UnitNormal(Vector3d& v1, const Vector3d& v2)
{
	return Unit(v1 | v2) ;
}
const Vector3d UnitNormal(const Vector3d& v1, Vector3d& v2)
{
	return Unit(v1 | v2) ;
}
*/
Vector3d UnitNormal(const Vector3d& v1, const Vector3d& v2)
{
	return Unit(v1 | v2) ;
}

/*
const Vector3d operator |(Vector3d& v1, Vector3d& v2)
{
	return Vector3d(v1.Y()*v2.Z() - v1.Z()*v2.Y(),
			v1.Z()*v2.X() - v1.X()*v2.Z(),
			v1.X()*v2.Y() - v1.Y()*v2.X()) ;
}
const Vector3d operator |(Vector3d& v1, const Vector3d& v2)
{
	return Vector3d(v1.Y()*v2.Z() - v1.Z()*v2.Y(),
			v1.Z()*v2.X() - v1.X()*v2.Z(),
			v1.X()*v2.Y() - v1.Y()*v2.X()) ;
}
const Vector3d operator |(const Vector3d& v1, Vector3d& v2)
{
	return Vector3d(v1.Y()*v2.Z() - v1.Z()*v2.Y(),
			v1.Z()*v2.X() - v1.X()*v2.Z(),
			v1.X()*v2.Y() - v1.Y()*v2.X()) ;
}
*/
Vector3d operator |(const Vector3d& v1, const Vector3d& v2)
{
	return Vector3d(v1.Y()*v2.Z() - v1.Z()*v2.Y(),
			v1.Z()*v2.X() - v1.X()*v2.Z(),
			v1.X()*v2.Y() - v1.Y()*v2.X()) ;
}

// 行列で座標変換
Vector3d& Vector3d::Transform(const GLfloat m[16])
{
	int      i ;
	for (i = 0 ; i < 3 ; i ++)
		v[i] = m[i]*v[0] + m[4+i]*v[1] + m[8+i]*v[2] + m[12+i] ;
	return *this ;
}

// 行列で座標変換（回転／拡大縮小部分のみの演算 法線ベクトル変換用）
Vector3d& Vector3d::Transform3x3(const GLfloat m[16])
{
	int      i ;
	for (i = 0 ; i < 3 ; i ++)
		v[i] = m[i]*v[0] + m[4+i]*v[1] + m[8+i]*v[2] ;
	return *this ;
}

// Sphere Mapping 変換
Vector3d& Vector3d::SphericalVertex()
{
	GLfloat m = 2.0f * sqrt(X()*X() + Y()*Y() + (Z() + 1.0f)*(Z() + 1.0f)) ;

	if (m != 0.0f)
	{
		X() = X() / m + 0.5f ;
		Y() = Y() / m + 0.5f ;
	}
	else
	{
		X() = 0.0f ;
		Y() = 0.5f ;
	}

	return *this ;
}

Vector3d SphericalVertex(Vector3d vec)
{
	return (vec.SphericalVertex()) ;
}


// カラーと見なした際の彩度を変化
Vector3d& Vector3d::ChangeSaturation(float vividness)
{
	if (vividness == 1.0f) return *this ;

	// 彩度ゼロの場合の RGB 輝度（0〜255）
	float bright = v[0] * GRAY_RATIO_R + v[1] * GRAY_RATIO_G + v[2] * GRAY_RATIO_B ;
	if (bright == 0.0f) return *this ;

	// マイナスだけはチェックするためにクランプ
	v[0] = Clump((bright - v[0]) * (1.0f - vividness) + v[0], 0.0f, 100.0f) ;
	v[1] = Clump((bright - v[1]) * (1.0f - vividness) + v[1], 0.0f, 100.0f) ;
	v[2] = Clump((bright - v[2]) * (1.0f - vividness) + v[2], 0.0f, 100.0f) ;

	// 元々の bright の明るさに補正
	float ratio = v[0] * GRAY_RATIO_R + v[1] * GRAY_RATIO_G + v[2] * GRAY_RATIO_B ;
	ratio /= bright ;
	v[0] *= ratio ;
	v[1] *= ratio ;
	v[2] *= ratio ;

	return *this ;
}

Vector3d ChangeSaturation(Vector3d src, float vividness)
{
	return (src.ChangeSaturation(vividness)) ;
}


ostream& operator <<(ostream& os, const Vector3d& vec)
{
	os << vec.X() << ", " << vec.Y() << ", " << vec.Z() ;
	return os ;
}


Object& Object::MakePolygon(int polNo, int v1, int v2, int v3, const GLfloat data[][3], const GLfloat m[16])
{
	polygon[polNo].vertex[0].X() = data[v1][0] ;
	polygon[polNo].vertex[0].Y() = data[v1][1] ;
	polygon[polNo].vertex[0].Z() = data[v1][2] ;
	if (m) polygon[polNo].vertex[0].Transform(m) ;

	polygon[polNo].vertex[1].X() = data[v2][0] ;
	polygon[polNo].vertex[1].Y() = data[v2][1] ;
	polygon[polNo].vertex[1].Z() = data[v2][2] ;
	if (m) polygon[polNo].vertex[1].Transform(m) ;

	polygon[polNo].vertex[2].X() = data[v3][0] ;
	polygon[polNo].vertex[2].Y() = data[v3][1] ;
	polygon[polNo].vertex[2].Z() = data[v3][2] ;
	if (m) polygon[polNo].vertex[2].Transform(m) ;

	polygon[polNo].pNormal   = polygon[polNo].normal[0] =
	polygon[polNo].normal[1] = polygon[polNo].normal[2] =
		UnitNormal(polygon[polNo].vertex[1] - polygon[polNo].vertex[0],
			   polygon[polNo].vertex[2] - polygon[polNo].vertex[1]) ;

	return *this ;
}

Object& Object::MakePolygon(int polNo, int v1, int v2, int v3, int v4, const GLfloat data[][3], const GLfloat m[16])
{
	polygon[polNo].vertex[0].X() = data[v1][0] ;
	polygon[polNo].vertex[0].Y() = data[v1][1] ;
	polygon[polNo].vertex[0].Z() = data[v1][2] ;
	if (m) polygon[polNo].vertex[0].Transform(m) ;

	polygon[polNo].vertex[1].X() = data[v2][0] ;
	polygon[polNo].vertex[1].Y() = data[v2][1] ;
	polygon[polNo].vertex[1].Z() = data[v2][2] ;
	if (m) polygon[polNo].vertex[1].Transform(m) ;

	polygon[polNo].vertex[2].X() = data[v3][0] ;
	polygon[polNo].vertex[2].Y() = data[v3][1] ;
	polygon[polNo].vertex[2].Z() = data[v3][2] ;
	if (m) polygon[polNo].vertex[2].Transform(m) ;

	polygon[polNo].vertex[3].X() = data[v4][0] ;
	polygon[polNo].vertex[3].Y() = data[v4][1] ;
	polygon[polNo].vertex[3].Z() = data[v4][2] ;
	if (m) polygon[polNo].vertex[3].Transform(m) ;

	polygon[polNo].pNormal   = polygon[polNo].normal[0] =
	polygon[polNo].normal[1] = polygon[polNo].normal[2] =
	polygon[polNo].normal[3] = 
		UnitNormal(polygon[polNo].vertex[1] - polygon[polNo].vertex[0],
			   polygon[polNo].vertex[2] - polygon[polNo].vertex[1]) ;

	return *this ;
}

Object& Object::AverageNormal(int pNo, int vNo, int pNo1, int pNo2, int pNo3, int pNo4, int pNo5, int pNo6)
{
	Vector3d sum = polygon[pNo1].pNormal + polygon[pNo2].pNormal ;

	if (pNo3 >= 0) sum += polygon[pNo3].pNormal ;
	if (pNo4 >= 0) sum += polygon[pNo4].pNormal ;
	if (pNo5 >= 0) sum += polygon[pNo5].pNormal ;
	if (pNo6 >= 0) sum += polygon[pNo6].pNormal ;

	polygon[pNo].normal[vNo] = Unit(sum) ;

	return *this ;
}

Object& Object::DrawTriangle(int p, int depth, int average, int flatFlag, int normalFlag, float *s, float *t)
{
	int      i, pFlag = TRUE ;	// 法線共有フラグ
	GLfloat vtx[6][3] ;
	Object   obj(5) ;

	// フラット指定または、各頂点の法線が全て同じ場合
	if (flatFlag || (polygon[p].normal[0] == polygon[p].normal[1] &&
					 polygon[p].normal[1] == polygon[p].normal[2])) pFlag = FALSE ;

	if (depth <= 0)
	{
		if (flatFlag)	// フラット時は、DrawTriangles() で、glBegin() 済み
		{
			// 99-09-23		// フラットで頂点平均化なしの場合、DrawTriangles() で glNormal 済み
			if (average && normalFlag) glNormal3fv(polygon[p].normal[0].v) ;

			if (s && t)
			{
				Vector3d& v = polygon[p].vertex[0] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(polygon[p].vertex[0].v) ;

			if (s && t)
			{
				Vector3d& v = polygon[p].vertex[1] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(polygon[p].vertex[1].v) ;

			if (s && t)
			{
				Vector3d& v = polygon[p].vertex[2] ;
				TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
			}
			glVertex3fv(polygon[p].vertex[2].v) ;
		}
		else	// フラット指定なし時（glBegin(), glNormal() 必要）
		{
			// ここまで来るのは、法線が全て同じポリゴンか
			// 初めから depth = 0 に指定されたもの
			// 法線が違う場合（スムーズシェーディング用）は、基本的に depth == 1 の時に処理）
			glBegin(GL_TRIANGLES) ;
			{
				if (normalFlag) glNormal3fv(polygon[p].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[p].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[p].vertex[0].v) ;

				// 最初から depth == 0 のものは、法線が頂点毎に違う可能性あり
				if (pFlag && normalFlag) glNormal3fv(polygon[p].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[p].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[p].vertex[1].v) ;

				if (pFlag && normalFlag) glNormal3fv(polygon[p].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[p].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[p].vertex[2].v) ;
			}
			glEnd() ;
		}
		return *this ;
	}

	// ポリゴン細分化処理

	VectorToArray(vtx[0], polygon[p].vertex[0]) ;
	VectorToArray(vtx[1], polygon[p].vertex[1]) ;
	VectorToArray(vtx[2], polygon[p].vertex[2]) ;

	if (average)
	{
		float a3 = (polygon[p].vertex[0].Abs() + polygon[p].vertex[1].Abs()) / 2.0f,
			   a4 = (polygon[p].vertex[1].Abs() + polygon[p].vertex[2].Abs()) / 2.0f,
			   a5 = (polygon[p].vertex[2].Abs() + polygon[p].vertex[0].Abs()) / 2.0f ;

		VectorToArray(vtx[3], Unit(polygon[p].vertex[0] + polygon[p].vertex[1]) * a3) ;
		VectorToArray(vtx[4], Unit(polygon[p].vertex[1] + polygon[p].vertex[2]) * a4) ;
		VectorToArray(vtx[5], Unit(polygon[p].vertex[2] + polygon[p].vertex[0]) * a5) ;
	}
	else
	{

		VectorToArray(vtx[3], (polygon[p].vertex[0] + polygon[p].vertex[1]) / 2.0f) ;
		VectorToArray(vtx[4], (polygon[p].vertex[1] + polygon[p].vertex[2]) / 2.0f) ;
		VectorToArray(vtx[5], (polygon[p].vertex[2] + polygon[p].vertex[0]) / 2.0f) ;
	}

	obj.MakePolygon(0, 0,3,5, vtx) ;
	obj.MakePolygon(1, 3,1,4, vtx) ;
	obj.MakePolygon(2, 5,4,2, vtx) ;
	obj.MakePolygon(3, 3,4,5, vtx) ;

	// 分割前のポリゴンの法線が頂点毎に違う場合は平均化した法線を計算
	// 同じだった場合はポリゴン生成時に計算された法線をそのまま使用
	if (pFlag)
	{
		obj.polygon[0].normal[0] = polygon[p].normal[0] ;
		obj.polygon[1].normal[1] = polygon[p].normal[1] ;
		obj.polygon[2].normal[2] = polygon[p].normal[2] ;

		obj.polygon[0].normal[1] = obj.polygon[1].normal[0] = obj.polygon[3].normal[0] =
			Unit(polygon[p].normal[0] + polygon[p].normal[1]) ;
		obj.polygon[1].normal[2] = obj.polygon[2].normal[1] = obj.polygon[3].normal[1] =
			Unit(polygon[p].normal[1] + polygon[p].normal[2]) ;
		obj.polygon[0].normal[2] = obj.polygon[2].normal[0] = obj.polygon[3].normal[2] =
			Unit(polygon[p].normal[2] + polygon[p].normal[0]) ;
	}

	// depth == 1 で、flatFlag 指定がなく
	// なおかつ分割後の法線が隣のポリゴンと共有している場合（!average || pFlag）
	if (depth == 1 && !flatFlag && (!average || pFlag))
	{
// EXT_vertex_array がサポートされておらず、
// OpenGL-1.0 または DONOT_USE_GL_1_1_VERTEX_ARRAY が定義されている場合
//#if !defined GL_EXT_vertex_array && (!defined GL_VERSION_1_1 || defined DONOT_USE_GL_1_1_VERTEX_ARRAY)

		if (!pFlag)
		{
			glBegin(GL_TRIANGLES) ;

			for (int p = 0 ; p < 4 ; p ++)
			{
				if (normalFlag) glNormal3fv(obj.polygon[p].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[p].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[p].vertex[2].v) ;

				if (s && t)
				{
					Vector3d& v = obj.polygon[p].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[p].vertex[0].v) ;

				// 最初の頂点以外の法線は、法線の平均化を行っていない場合は必要なし
//				if (pFlag) glNormal3fv(obj.polygon[p].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[p].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[p].vertex[1].v) ;

			// 最初の頂点以外の法線は、法線の平均化を行っていない場合は必要なし
//				if (pFlag) glNormal3fv(obj.polygon[p].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[p].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[p].vertex[2].v) ;
			}
			glEnd() ;
		}
		else	// if (pFlag)
		{
			// GL_TRIANGLE_STRIP で３枚（0, 3, 1）+ GL_TRIANGLES で１枚（2）

			glBegin(GL_TRIANGLE_STRIP) ;

				if (normalFlag) glNormal3fv(obj.polygon[1].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[1].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[1].vertex[1].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[1].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[1].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[1].vertex[2].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[1].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[1].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[1].vertex[0].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[0].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[0].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[0].vertex[2].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[0].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[0].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[0].vertex[0].v) ;

			glEnd() ;

			glBegin(GL_TRIANGLES) ;

				if (normalFlag) glNormal3fv(obj.polygon[2].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[2].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[2].vertex[0].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[2].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[2].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[2].vertex[1].v) ;

				if (normalFlag) glNormal3fv(obj.polygon[2].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = obj.polygon[2].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(obj.polygon[2].vertex[2].v) ;

			glEnd() ;
		}
/*
		// GL_TRIANGLE_STRIP で３枚（0, 3, 1）+ GL_TRIANGLES で１枚（2）

		glBegin(GL_TRIANGLE_STRIP) ;

			glNormal3fv(obj.polygon[1].normal[1].v) ;
			glVertex3fv(polygon[1].vertex[1].v) ;

			// 最初の頂点以外の法線は、法線の平均化を行っていない場合は必要なし
			if (pFlag) glNormal3fv(obj.polygon[1].normal[2].v) ;
			glVertex3fv(obj.polygon[1].vertex[2].v) ;

			if (pFlag) glNormal3fv(obj.polygon[0].normal[1].v) ;
			glVertex3fv(obj.polygon[0].vertex[1].v) ;

			if (pFlag) glNormal3fv(obj.polygon[0].normal[2].v) ;
			glVertex3fv(obj.polygon[0].vertex[2].v) ;

			if (pFlag) glNormal3fv(obj.polygon[0].normal[0].v) ;
			glVertex3fv(obj.polygon[0].vertex[0].v) ;

		glEnd() ;

		glBegin(GL_TRIANGLES) ;

			glNormal3fv(obj.polygon[0].normal[0].v) ;
			glVertex3fv(obj.polygon[0].vertex[0].v) ;

			if (pFlag) glNormal3fv(obj.polygon[0].normal[1].v) ;
			glVertex3fv(obj.polygon[0].vertex[1].v) ;

			if (pFlag) glNormal3fv(obj.polygon[0].normal[2].v) ;
			glVertex3fv(obj.polygon[0].vertex[2].v) ;

		glEnd() ;
*/


/*
#else // OpenGL-1.1、または GL_EXT_vertex_array が使用可能な場合

		Vector3d normals[6] ;

		normals[0] = obj.polygon[0].normal[0] ;
		normals[1] = obj.polygon[1].normal[1] ;
		normals[2] = obj.polygon[2].normal[2] ;
		normals[3] = obj.polygon[0].normal[1] ;
		normals[4] = obj.polygon[1].normal[2] ;
		normals[5] = obj.polygon[0].normal[2] ;

// OpenGL-1.0 で GL_EXT_vertex_array がサポートされているか、
// DONOT_USE_GL_1_1_VERTEX_ARRAY が定義されている場合
#if (!defined GL_VERSION_1_1 && defined GL_EXT_vertex_array) || defined DONOT_USE_GL_1_1_VERTEX_ARRAY

		glEnable(GL_VERTEX_ARRAY_EXT) ;
		glEnable(GL_NORMAL_ARRAY_EXT) ;

DONOT_USE_GL_1_1_VERTEX_ARRAY;

		glVertexPointerEXT(3, GL_FLOAT, 0, 6, (GLfloat *)(&vtx[0][0])) ;
		glNormalPointerEXT(GL_FLOAT, sizeof(Vector3d), 6, (GLfloat *)(&normals[0].v[0])) ;

		glBegin(GL_TRIANGLES) ;
			glArrayElement(0) ;
			glArrayElement(3) ;
			glArrayElement(5) ;

			glArrayElement(3) ;
			glArrayElement(1) ;
			glArrayElement(4) ;

			glArrayElement(5) ;
			glArrayElement(4) ;
			glArrayElement(2) ;

			glArrayElement(3) ;
			glArrayElement(4) ;
			glArrayElement(5) ;
		glEnd() ;

#else

		GLubyte indices[] = { 0, 3, 5,  3, 1, 4,  5, 4, 2,  3, 4, 5 } ;

		glEnableClientState(GL_VERTEX_ARRAY) ;
		glEnableClientState(GL_NORMAL_ARRAY) ;

		glVertexPointer(3, GL_FLOAT, 0, (GLfloat *)(&vtx[0][0])) ;
		glNormalPointer(GL_FLOAT, sizeof(Vector3d), (GLfloat *)(&normals[0].v[0])) ;

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, indices) ;

#endif
#endif
*/
		return *this ;
	}

	for (i = 0 ; i < 4 ; i ++) obj.DrawTriangle(i, depth - 1, average, flatFlag, normalFlag, s, t) ;

	return *this ;
}

// flatFlag … フラットシェーディングと仮定
//		各ポリゴンを構成する頂点が全ておなじ法線を持つもの仮定して描画
// normalFlag
//		FALSE の時は、法線の設定を一切行わない
Object& Object::DrawTriangles(int depth, int average, int flatFlag, int normalFlag, float *s, float *t)
{
	int i ;

	// 細分化なしの場合は、平均化もなし
	if (depth <= 0) average = FALSE ;

	// フラットシェーディングと仮定された場合（つまり各ポリゴンの頂点毎の法線がすべて同じ）
	if (flatFlag)
	{
		glBegin(GL_TRIANGLES) ;

			for (i = 0 ; i < nPolygons ; i ++)
			{
				// フラットシェーディングで頂点座標を平均化しない場合は、最初に法線の０番を指定
				if (!average && normalFlag) glNormal3fv(polygon[i].normal[0].v) ;

				// 三角ポリゴン描画
				DrawTriangle(i, depth, average, TRUE /* flatFlag */, normalFlag, s, t) ;
			}

		glEnd() ;
	}
	else	// 通常の分割処理の場合
		for (i = 0 ; i < nPolygons ; i ++) DrawTriangle(i, depth, average, flatFlag /* FALSE */, normalFlag, s, t) ;

	return *this ;
}


RollingObject& RollingObject::MakeRollingPolygons(int u, int v, GLfloat data[][2], GLfloat x, GLfloat y, GLfloat z)
{
	int      i ;
	GLfloat (*vertex)[3] ;
	if (u < 3 || v < 2) return *this ;

	vertex = new GLfloat[v * 2][3] ;

	uDiv = u ;
	vDiv = v ;

	if (data[    0][0] == 0.0f) fType = 1 ;
	else                       fType = 0 ;
	if (data[v - 1][0] == 0.0f) eType = 1 ;
	else                       eType = 0 ;

	for (i = 0 ; i < u ; i ++)
	{
		int    j, pNo = i * (v - 1) ;
		float rad = 2.0f * PAI / u * i, pRad = 2.0f * PAI / u * (i - 1) ;

		for (j = 0 ; j < v ; j ++)
		{
			vertex[j * 2    ][0] = data[j][0] * x * sin( rad) ;
			vertex[j * 2    ][1] = data[j][0] * y * cos( rad) ;
			vertex[j * 2    ][2] = data[j][1] * z ;

			vertex[j * 2 + 1][0] = data[j][0] * x * sin(pRad) ;
			vertex[j * 2 + 1][1] = data[j][0] * y * cos(pRad) ;
			vertex[j * 2 + 1][2] = data[j][1] * z ;
		}

		if (fType) MakePolygon(pNo        , 0          , 2          , 3        , vertex) ;
		if (eType) MakePolygon(pNo + v - 2, (v - 2) * 2, (v - 1) * 2, v * 2 - 3, vertex) ;

		for (j = fType ; j < v - 1 - eType ; j ++)
			MakePolygon(pNo + j, j * 2, j * 2 + 2, j * 2 + 3, j * 2 + 1, vertex) ;
	}

	delete [] vertex ;
	return *this ;
}

int RollingObject::VFlag(int v, int ud, int vFlag, int vFlags[][2])
{
	if (vFlag)
	{
		if (vFlags) return vFlags[v][ud] ;
		else        return TRUE ;
	}
	else            return FALSE ;
}

RollingObject& RollingObject::AverageNormals(int uFlag, int vFlag, int vFlags[][2]) //, int vNo)
{
	int u ;
	if (uDiv < 3 || vDiv < 2) return *this ;

	for (u = 0 ; u < uDiv ; u ++)
	{
		int v, pNo = u * (vDiv - 1), pPNo = (u - 1) * (vDiv - 1), nPNo = (u + 1) * (vDiv - 1) ;

		if      (u ==        0) pPNo = (uDiv - 1) * (vDiv - 1) ;
		else if (u == uDiv - 1) nPNo = 0 ;

		for (v = 0 /*vNo*/ ; v < vDiv - 1 ; v ++)
		{
			int p = pNo + v, pp = pPNo + v, np = nPNo + v ;

			// Vertex 0
			if (!v && uFlag)
			{
				if (fType) AverageNormal(p, 0, p,np, pp) ;
				else       AverageNormal(p, 0, p,np) ;
			}
			else if (uFlag)
			{
				if (VFlag(v, 0, vFlag, vFlags)) AverageNormal(p, 0, p,np,p-1,np-1) ;
				else                            AverageNormal(p, 0, p,np) ;
			}
			else if (!uFlag && VFlag(v, 0, vFlag, vFlags)) AverageNormal(p, 0, p,p-1) ;

			// Vertex 1
			if (v == vDiv - 2 && uFlag)
			{
				if (eType) AverageNormal(p, 1, p,np,pp) ;
				else       AverageNormal(p, 1, p,np) ;
			}
			else if (uFlag)
			{
				if (VFlag(v, 1, vFlag, vFlags)) AverageNormal(p, 1, p,np,p+1,np+1) ;
				else                            AverageNormal(p, 1, p,np) ;
			}
			else if (!uFlag && VFlag(v, 1, vFlag, vFlags)) AverageNormal(p, 1, p,p+1) ;

			// Vertex 2
			if (v == vDiv - 2 && uFlag)
			{
				if (eType && VFlag(v, 1, vFlag, vFlags)) AverageNormal(p, 2, p,pp,p-1,pp-1) ;
				else                                     AverageNormal(p, 2, p,pp) ;
			}
			else if (uFlag)
			{
				if (VFlag(v, 1, vFlag, vFlags)) AverageNormal(p, 2, p,pp,p+1,pp+1) ;
				else                            AverageNormal(p, 2, p,pp) ;
			}
			else if (!uFlag && VFlag(v, 1, vFlag, vFlags)) AverageNormal(p, 2, p,p+1) ;

			// Vertex 3
			if (!v && uFlag) { if (!fType) AverageNormal(p, 3, p,pp) ; }
			else if (uFlag)
			{
				if (VFlag(v, 0, vFlag, vFlags)) AverageNormal(p, 3, p,pp,p-1,pp-1) ;
				else                            AverageNormal(p, 3, p,pp) ;
			}
			else if (!uFlag && VFlag(v, 0, vFlag, vFlags)) AverageNormal(p, 3, p,p-1) ;
		}
	}
	return *this ;
}

RollingObject& RollingObject::TopNormal(GLfloat x, GLfloat y, GLfloat z)
{
	if (!fType) return *this ;

	int u ;
	for (u = 0 ; u < uDiv ; u ++)
	{
		int pNo = u * (vDiv - 1) ;

		polygon[pNo].normal[0].v[0] = x ;
		polygon[pNo].normal[0].v[1] = y ;
		polygon[pNo].normal[0].v[2] = z ;
	}

	return *this ;
}

RollingObject& RollingObject::BottomNormal(GLfloat x, GLfloat y, GLfloat z)
{
	if (!eType) return *this ;

	int u ;
	for (u = 0 ; u < uDiv ; u ++)
	{
		int pNo = u * (vDiv - 1) + vDiv - 2 ;

		polygon[pNo].normal[1].v[0] = x ;
		polygon[pNo].normal[1].v[1] = y ;
		polygon[pNo].normal[1].v[2] = z ;
	}

	return *this ;
}


// 回転体オブジェクトの描画（法線、頂点座標）
// normalFlag が FALSE の時は、法線を一切セットしない
void RollingObject::DrawObject(int normalFlag, float *s, float *t)
{
	int u, v ;

	for (v = 0 ; v < vDiv - 1 ; v ++)
	{
		if (v == 0 && fType) // てっぺんの三角形
		{
			int pNo = (uDiv - 1) * (vDiv - 1) ;

			glBegin(GL_TRIANGLE_FAN) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[0].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[1].v) ;

				for (u = uDiv - 1 ; u >= 0 ; u --)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[2] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[2].v) ;

					pNo -= vDiv - 1 ;
				}

			glEnd() ;
		}
		else if (v == vDiv - 2 && eType) // 底の三角形
		{
			int pNo = v ;

			glBegin(GL_TRIANGLE_FAN) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[1].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[2].v) ;

				for (u = 0 ; u < uDiv ; u ++)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[0] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[0].v) ;

					pNo += vDiv - 1 ;
				}

			glEnd() ;
		}
		else  // 通常の四角形
		{
			int pNo = v ;

			glBegin(GL_QUAD_STRIP) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[2].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[3].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[3] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[3].v) ;

				for (u = 0 ; u < uDiv ; u ++)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[1] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[1].v) ;

					if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[0] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[0].v) ;

					pNo += vDiv - 1 ;
				}

			glEnd() ;
		}
	}
}


PositivePolyhedron& PositivePolyhedron::Smooth()
{
	int i, j ;
	
	for (i = 0 ; i < nPolygons ; i ++)
		for (j = 0 ; j < 3 ; j ++)
			polygon[i].normal[j] = Unit(polygon[i].vertex[j]) ;

	return *this ;
}


Octahedron& Octahedron::Make(GLfloat x, GLfloat y, GLfloat z, int d, int a, int smooth)
{
	int      i ;
	GLfloat m[] = {   x, 0.0f, 0.0f, 0.0f,
					 0.0f,   y, 0.0f, 0.0f,
					 0.0f, 0.0f,   z, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f } ;

	if (d >= 0) depth   = d ;
	if (a >= 0) average = a ;

	NPolygons(8) ;
	for (i = 0 ; i < 8 ; i ++) MakePolygon(i, o_o[i][0],o_o[i][1],o_o[i][2], o_v, m) ;

	if (smooth) Smooth() ;

	return *this ;
}

void Octahedron::DrawPolyhedron(GLfloat r, int normalFlag, float *s, float *t)
{
	GLfloat v[6][3] ;

	for (int i = 0 ; i < 6 ; i ++)
	{
		v[i][0] = o_v[i][0] * r ;
		v[i][1] = o_v[i][1] * r ;
		v[i][2] = o_v[i][2] * r ;
	}

	Octahedron obj(r, r, r) ;
/*
#if defined GL_VERSION_1_1 && !defined DONOT_USE_GL_1_1_VERTEX_ARRAY
	glEnableClientState(GL_VERTEX_ARRAY) ;
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat *)(&v[0][0])) ;
#endif
*/

	if (normalFlag)
	{
		glBegin(GL_TRIANGLES) ;
		{
			for (int i = 0 ; i < 8 ; i ++)
			{
				if (normalFlag) glNormal3fv(obj.polygon[i].normal[0].v) ;
/*
#if defined GL_VERSION_1_1 && !defined DONOT_USE_GL_1_1_VERTEX_ARRAY
				glArrayElement(o_o[i][0]) ;
				glArrayElement(o_o[i][1]) ;
				glArrayElement(o_o[i][2]) ;
#else
*/
				if (s && t)
				{
					float *vt = v[o_o[i][0]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[o_o[i][0]]) ;

				if (s && t)
				{
					float *vt = v[o_o[i][1]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[o_o[i][1]]) ;

				if (s && t)
				{
					float *vt = v[o_o[i][2]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[o_o[i][2]]) ;
/*
#endif
*/
			}
		}
		glEnd() ;
	}
	else
	{
		// 法線情報不要の場合は FAN で最適化
		glBegin(GL_TRIANGLE_FAN) ;
		{
			// 上頂点
			if (s && t)
			{
				float *vt = v[4] ;
				TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
			}
			glVertex3fv(v[4]) ;

			for (int i = 0 ; i < 5 ; i ++)
			{
				int n = i % 4 ;
				if (s && t)
				{
					float *vt = v[n] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[n]) ;
			}
		}
		glEnd() ;

		glBegin(GL_TRIANGLE_FAN) ;
		{
			// 下頂点
			if (s && t)
			{
				float *vt = v[5] ;
				TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
			}
			glVertex3fv(v[5]) ;

			for (int i = 0 ; i < 5 ; i ++)
			{
				int n = (4 - i) % 4 ;
				if (s && t)
				{
					float *vt = v[n] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[n]) ;
			}
		}
		glEnd() ;
	}
}

GLfloat Octahedron::o_v[][3] = { {  0.0f, 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, {  0.0f,-1.0f, 0.0f },
								 {  1.0f, 0.0f, 0.0f }, {  0.0f, 0.0f, 1.0f }, {  0.0f, 0.0f,-1.0f } } ;

int Octahedron::o_o[][3] = { { 0, 1, 4 }, { 1, 2, 4 }, { 2, 3, 4 }, { 3, 0, 4 },
							 { 1, 0, 5 }, { 2, 1, 5 }, { 3, 2, 5 }, { 0, 3, 5 } } ;


Icosahedron& Icosahedron::Make(GLfloat x, GLfloat y, GLfloat z, int d, int a, int smooth)
{
	int      i ;
	GLfloat m[] = {   x, 0.0f, 0.0f, 0.0f,
					 0.0f,   y, 0.0f, 0.0f,
					 0.0f, 0.0f,   z, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f } ;

	if (d >= 0) depth   = d ;
	if (a >= 0) average = a ;

	NPolygons(20) ;
	for (i = 0 ; i < 20 ; i ++) MakePolygon(i, i_o[i][2],i_o[i][1],i_o[i][0], i_v, m) ;

	if (smooth) Smooth() ;

	return *this ;
}

void Icosahedron::DrawPolyhedron(GLfloat r, int normalFlag, float *s, float *t)
{
	GLfloat v[12][3] ;

	for (int i = 0 ; i < 12 ; i ++)
	{
		v[i][0] = i_v[i][0] * r ;
		v[i][1] = i_v[i][1] * r ;
		v[i][2] = i_v[i][2] * r ;
	}

	Icosahedron obj(r, r, r) ;
/*
#if defined GL_VERSION_1_1 && !defined DONOT_USE_GL_1_1_VERTEX_ARRAY
	glEnableClientState(GL_VERTEX_ARRAY) ;
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat *)(&v[0][0])) ;
#endif
*/

	if (normalFlag)
	{
		glBegin(GL_TRIANGLES) ;
		{
			for (int i = 0 ; i < 20 ; i ++)
			{
				if (normalFlag) glNormal3fv(obj.polygon[i].normal[0].v) ;
/*
#if defined GL_VERSION_1_1 && !defined DONOT_USE_GL_1_1_VERTEX_ARRAY
				glArrayElement(i_o[i][2]) ;
				glArrayElement(i_o[i][1]) ;
				glArrayElement(i_o[i][0]) ;
#else
*/
				if (s && t)
				{
					float *vt = v[i_o[i][2]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[i_o[i][2]]) ;

				if (s && t)
				{
					float *vt = v[i_o[i][1]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[i_o[i][1]]) ;

				if (s && t)
				{
					float *vt = v[i_o[i][0]] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[i_o[i][0]]) ;
/*
#endif
*/
			}
		}
		glEnd() ;
	}
	else
	{
		int tfan1[ 7] = { 4,  0, 1, 8, 5, 9, 0 } ;
		int strip[12] = { 0, 6, 1,10, 8, 3, 5, 2, 9,11, 0, 6 } ;
		int tfan2[ 7] = { 7,  3,10, 6,11, 2, 3 } ;

		// 法線情報不要の場合は FAN と STRIP で最適化
		glBegin(GL_TRIANGLE_FAN) ;
		{
			for (int i = 0 ; i < 7 ; i ++)
			{
				int n = tfan1[i] ;
				if (s && t)
				{
					float *vt = v[n] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[n]) ;
			}
		}
		glEnd() ;

		glBegin(GL_TRIANGLE_STRIP) ;
		{
			for (int i = 0 ; i < 12 ; i ++)
			{
				int n = strip[i] ;
				if (s && t)
				{
					float *vt = v[n] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[n]) ;
			}
		}
		glEnd() ;

		glBegin(GL_TRIANGLE_FAN) ;
		{
			for (int i = 0 ; i < 7 ; i ++)
			{
				int n = tfan2[i] ;
				if (s && t)
				{
					float *vt = v[n] ;
					TexCoord2f(s[0] * vt[0] + s[1] * vt[1] + s[2] * vt[3], t[0] * vt[0] + t[1] * vt[1] + t[2] * vt[2]) ;
				}
				glVertex3fv(v[n]) ;
			}
		}
		glEnd() ;
	}
}


// 屈折描画（屈折関係データ計算、OpenGL 描画処理）
// 環境マップ描画はまた別
// glEnable(GL_TEXTURE_2D) ;
// glDisable(GL_LIGHTING) ;
RefractRollingObject& RefractRollingObject::DrawRefractObject(const Vector3d& r_c, const Vector3d& s_c)
{
	int     u, v ;
	GLfloat matrix[16] ;

	// 現在のモデルビュー行列ゲット
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix) ;

	// 頂点単位の屈折データ（テクスチャ座標、透過、反射色計算）
	Vertex(0, 0).CalculateVertex(matrix, r_c, s_c) ; // てっぺんの頂点
	Vertex(0, vDiv - 1).CalculateVertex(matrix, r_c, s_c) ; // 底の頂点

	// それ以外（通常の回転頂点）
	for (v = 0 ; v < vDiv - 1 ; v ++)
	{
		for (u = 0 ; u < uDiv ; u ++)
			Vertex(u, v).CalculateVertex(matrix, r_c, s_c) ;

		// 一周して戻ってきた頂点は最初と同じ
		Vertex(uDiv, v) = Vertex(0, v) ;
	}

	// OpenGL 描画

	// てっぺんの三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glColor3fv(Vertex(0, 0).refraction_color.v) ;
		glTexCoord2fv(Vertex(0, 0).texture.v) ;
		glVertex3fv(Vertex(0, 0).vertex.v) ;

		for (u = 0 ; u <= uDiv ; u ++)
		{
			glColor3fv(Vertex(u, 1).refraction_color.v) ;
			glTexCoord2fv(Vertex(u, 1).texture.v) ;
			glVertex3fv(Vertex(u, 1).vertex.v) ;
		}
	}
	glEnd() ;

	// 底の三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glColor3fv(Vertex(0, vDiv - 1).refraction_color.v) ;
		glTexCoord2fv(Vertex(0, vDiv - 1).texture.v) ;
		glVertex3fv(Vertex(0, vDiv - 1).vertex.v) ;

		for (u = uDiv ; u >= 0 ; u --)
		{
			glColor3fv(Vertex(u, vDiv - 2).refraction_color.v) ;
			glTexCoord2fv(Vertex(u, vDiv - 2).texture.v) ;
			glVertex3fv(Vertex(u, vDiv - 2).vertex.v) ;
		}
	}
	glEnd() ;

	// それ以外（通常の QUAD_STRIP）
	for (v = 1 ; v < vDiv - 1 ; v ++)
	{
		glBegin(GL_QUAD_STRIP) ;
		{
			for (u = 0 ; u <= uDiv ; u ++)
			{
				// 上
				glColor3fv(Vertex(u, v).refraction_color.v) ;
				glTexCoord2fv(Vertex(u, v).texture.v) ;
				glVertex3fv(Vertex(u, v).vertex.v) ;
				// 下
				glColor3fv(Vertex(u, v + 1).refraction_color.v) ;
				glTexCoord2fv(Vertex(u, v + 1).texture.v) ;
				glVertex3fv(Vertex(u, v + 1).vertex.v) ;
			}
		}
		glEnd() ;
	}

	return *this ;
}

// 屈折描画（屈折関係データ計算、OpenGL 描画処理）
// 環境マップ描画はまた別
// 透過率、反射率は計算しない（アプリケーションが自前で glColor() を実行）
// glEnable(GL_TEXTURE_2D) ;
// glDisable(GL_LIGHTING) ;
// glColor*() ;
RefractRollingObject& RefractRollingObject::DrawRefractObject()
{
	int     u, v ;
	GLfloat matrix[16] ;

	// 現在のモデルビュー行列ゲット
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix) ;

	// 頂点単位の屈折データ（テクスチャ座標、透過、反射色計算）
	Vertex(0, 0).CalculateVertex(matrix) ; // てっぺんの頂点
	Vertex(0, vDiv - 1).CalculateVertex(matrix) ; // 底の頂点

	// それ以外（通常の回転頂点）
	for (v = 0 ; v < vDiv - 1 ; v ++)
	{
		for (u = 0 ; u < uDiv ; u ++)
			Vertex(u, v).CalculateVertex(matrix) ;

		// 一周して戻ってきた頂点は最初と同じ
		Vertex(uDiv, v) = Vertex(0, v) ;
	}

	// OpenGL 描画

	// てっぺんの三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glTexCoord2fv(Vertex(0, 0).texture.v) ;
		glVertex3fv(Vertex(0, 0).vertex.v) ;

		for (u = 0 ; u <= uDiv ; u ++)
		{
			glTexCoord2fv(Vertex(u, 1).texture.v) ;
			glVertex3fv(Vertex(u, 1).vertex.v) ;
		}
	}
	glEnd() ;

	// 底の三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glTexCoord2fv(Vertex(0, vDiv - 1).texture.v) ;
		glVertex3fv(Vertex(0, vDiv - 1).vertex.v) ;

		for (u = uDiv ; u >= 0 ; u --)
		{
			glTexCoord2fv(Vertex(u, vDiv - 2).texture.v) ;
			glVertex3fv(Vertex(u, vDiv - 2).vertex.v) ;
		}
	}
	glEnd() ;

	// それ以外（通常の QUAD_STRIP）
	for (v = 1 ; v < vDiv - 2 ; v ++)
	{
		glBegin(GL_QUAD_STRIP) ;
		{
			for (u = 0 ; u <= uDiv ; u ++)
			{
				// 上
				glTexCoord2fv(Vertex(u, v).texture.v) ;
				glVertex3fv(Vertex(u, v).vertex.v) ;
				// 下
				glTexCoord2fv(Vertex(u, v + 1).texture.v) ;
				glVertex3fv(Vertex(u, v + 1).vertex.v) ;
			}
		}
		glEnd() ;
	}

	return *this ;
}


// 屈折描画（我流フェイク屈折計算、OpenGL 描画処理）
// 環境マップ描画はまた別
// glEnable(GL_TEXTURE_2D) ;
// glDisable(GL_LIGHTING) ;
RefractRollingObject& RefractRollingObject::DrawFakeRefractObject(const Vector3d& r_c, const Vector3d& s_c)
{
	int     u, v ;
	GLfloat matrix[16] ;

	// 現在のモデルビュー行列ゲット
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix) ;

	// 頂点単位の屈折データ（テクスチャ座標、透過、反射色計算）
	Vertex(0, 0).CalculateFakeVertex(matrix, r_c, s_c) ; // てっぺんの頂点
	Vertex(0, vDiv - 1).CalculateFakeVertex(matrix, r_c, s_c) ; // 底の頂点

	// それ以外（通常の回転頂点）
	for (v = 0 ; v < vDiv - 1 ; v ++)
	{
		for (u = 0 ; u < uDiv ; u ++)
			Vertex(u, v).CalculateFakeVertex(matrix, r_c, s_c) ;

		// 一周して戻ってきた頂点は最初と同じ
		Vertex(uDiv, v) = Vertex(0, v) ;
	}

	// OpenGL 描画

	// てっぺんの三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glColor3fv(Vertex(0, 0).refraction_color.v) ;
		glTexCoord2fv(Vertex(0, 0).texture.v) ;
		glVertex3fv(Vertex(0, 0).vertex.v) ;

		for (u = 0 ; u <= uDiv ; u ++)
		{
			glColor3fv(Vertex(u, 1).refraction_color.v) ;
			glTexCoord2fv(Vertex(u, 1).texture.v) ;
			glVertex3fv(Vertex(u, 1).vertex.v) ;
		}
	}
	glEnd() ;

	// 底の三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glColor3fv(Vertex(0, vDiv - 1).refraction_color.v) ;
		glTexCoord2fv(Vertex(0, vDiv - 1).texture.v) ;
		glVertex3fv(Vertex(0, vDiv - 1).vertex.v) ;

		for (u = uDiv ; u >= 0 ; u --)
		{
			glColor3fv(Vertex(u, vDiv - 2).refraction_color.v) ;
			glTexCoord2fv(Vertex(u, vDiv - 2).texture.v) ;
			glVertex3fv(Vertex(u, vDiv - 2).vertex.v) ;
		}
	}
	glEnd() ;

	// それ以外（通常の QUAD_STRIP）
	for (v = 1 ; v < vDiv - 1 ; v ++)
	{
		glBegin(GL_QUAD_STRIP) ;
		{
			for (u = 0 ; u <= uDiv ; u ++)
			{
				// 上
				glColor3fv(Vertex(u, v).refraction_color.v) ;
				glTexCoord2fv(Vertex(u, v).texture.v) ;
				glVertex3fv(Vertex(u, v).vertex.v) ;
				// 下
				glColor3fv(Vertex(u, v + 1).refraction_color.v) ;
				glTexCoord2fv(Vertex(u, v + 1).texture.v) ;
				glVertex3fv(Vertex(u, v + 1).vertex.v) ;
			}
		}
		glEnd() ;
	}

	return *this ;
}


// 屈折描画（屈折関係データ計算、OpenGL 描画処理）
// 環境マップ描画はまた別
// 透過率、反射率は計算しない（アプリケーションが自前で glColor() を実行）
// glEnable(GL_TEXTURE_2D) ;
// glDisable(GL_LIGHTING) ;
// glColor*() ;
RefractRollingObject& RefractRollingObject::DrawFakeRefractObject()
{
	int     u, v ;
	GLfloat matrix[16] ;

	// 現在のモデルビュー行列ゲット
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix) ;

	// 頂点単位の屈折データ（テクスチャ座標、透過、反射色計算）
	Vertex(0, 0).CalculateFakeVertex(matrix) ; // てっぺんの頂点
	Vertex(0, vDiv - 1).CalculateFakeVertex(matrix) ; // 底の頂点

	// それ以外（通常の回転頂点）
	for (v = 0 ; v < vDiv - 1 ; v ++)
	{
		for (u = 0 ; u < uDiv ; u ++)
			Vertex(u, v).CalculateFakeVertex(matrix) ;

		// 一周して戻ってきた頂点は最初と同じ
		Vertex(uDiv, v) = Vertex(0, v) ;
	}

	// OpenGL 描画

	// てっぺんの三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glTexCoord2fv(Vertex(0, 0).texture.v) ;
		glVertex3fv(Vertex(0, 0).vertex.v) ;

		for (u = 0 ; u <= uDiv ; u ++)
		{
			glTexCoord2fv(Vertex(u, 1).texture.v) ;
			glVertex3fv(Vertex(u, 1).vertex.v) ;
		}
	}
	glEnd() ;

	// 底の三角形ファン
	glBegin(GL_TRIANGLE_FAN) ;
	{
		glTexCoord2fv(Vertex(0, vDiv - 1).texture.v) ;
		glVertex3fv(Vertex(0, vDiv - 1).vertex.v) ;

		for (u = uDiv ; u >= 0 ; u --)
		{
			glTexCoord2fv(Vertex(u, vDiv - 2).texture.v) ;
			glVertex3fv(Vertex(u, vDiv - 2).vertex.v) ;
		}
	}
	glEnd() ;

	// それ以外（通常の QUAD_STRIP）
	for (v = 1 ; v < vDiv - 2 ; v ++)
	{
		glBegin(GL_QUAD_STRIP) ;
		{
			for (u = 0 ; u <= uDiv ; u ++)
			{
				// 上
				glTexCoord2fv(Vertex(u, v).texture.v) ;
				glVertex3fv(Vertex(u, v).vertex.v) ;
				// 下
				glTexCoord2fv(Vertex(u, v + 1).texture.v) ;
				glVertex3fv(Vertex(u, v + 1).vertex.v) ;
			}
		}
		glEnd() ;
	}

	return *this ;
}


// ワイングラス描画（法線、頂点座標）
void WineGlass::DrawObject(int normalFlag, float *s, float *t)
{
	int u, v ;

	for (v = 0 ; v < N_VERTICIES - 1 ; v ++)
	{
		if (v == 0) // てっぺんの三角形
		{
			int pNo = (uDiv - 1) * (N_VERTICIES - 1) ;

			glBegin(GL_TRIANGLE_FAN) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
//				glNormal3f(0.0, 0.0, 1.0) ;	// 一番上の頂点座標は法線を真上に
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[0] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[0].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[1].v) ;

				for (u = uDiv - 1 ; u >= 0 ; u --)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[2] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[2].v) ;

					pNo -= N_VERTICIES - 1 ;
				}

			glEnd() ;
		}
		else if (v == N_VERTICIES - 2 ) // 底の三角形
		{
			int pNo = v ;

			glBegin(GL_TRIANGLE_FAN) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
//				glNormal3d(0.0, 0.0, -1.0) ;	// 一番下の頂点座標は法線を真下に
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[1] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[1].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[2].v) ;

				for (u = 0 ; u < uDiv ; u ++)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[0] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[0].v) ;

					pNo += N_VERTICIES - 1 ;
				}

			glEnd() ;
		}
		else  // 通常の四角形
		{
			int pNo = v ;

			glBegin(GL_QUAD_STRIP) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[2].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[2] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[2].v) ;

				if (normalFlag) glNormal3fv(polygon[pNo].normal[3].v) ;
				if (s && t)
				{
					Vector3d& v = polygon[pNo].vertex[3] ;
					TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
				}
				glVertex3fv(polygon[pNo].vertex[3].v) ;

				for (u = 0 ; u < uDiv ; u ++)
				{
					if (normalFlag) glNormal3fv(polygon[pNo].normal[1].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[1] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[1].v) ;

					if (normalFlag) glNormal3fv(polygon[pNo].normal[0].v) ;
					if (s && t)
					{
						Vector3d& v = polygon[pNo].vertex[0] ;
						TexCoord2f(s[0] * v.X() + s[1] * v.Y() + s[2] * v.Z(), t[0] * v.X() + t[1] * v.Y() + t[2] * v.Z()) ;
					}
					glVertex3fv(polygon[pNo].vertex[0].v) ;

					pNo += N_VERTICIES - 1 ;
				}

			glEnd() ;
		}
	}
}

void WineGlass::DrawPolyhedron(int divide, GLfloat r, int normalFlag, float *s, float *t)
{
	WineGlass obj(divide, r,r,r) ;
	obj.DrawObject(normalFlag, s, t) ;
}


// 半球体の描画（法線ベクトル、頂点座標）
void HalfSphere::DrawObject()
{
	for (int v = 0 ; v < vDiv - 1 ; v ++)
	{
		int pNo = v ;

		glBegin(GL_QUAD_STRIP) ;

			glNormal3fv(polygon[pNo].normal[2].v) ;
			glVertex3fv(polygon[pNo].vertex[2].v) ;

			glNormal3fv(polygon[pNo].normal[3].v) ;
			glVertex3fv(polygon[pNo].vertex[3].v) ;

			for (int u = 0 ; u < uDiv ; u ++)
			{
				glNormal3fv(polygon[pNo].normal[1].v) ;
				glVertex3fv(polygon[pNo].vertex[1].v) ;

				glNormal3fv(polygon[pNo].normal[0].v) ;
				glVertex3fv(polygon[pNo].vertex[0].v) ;

				pNo += vDiv - 1 ;
			}

		glEnd() ;
	}
}

// 半球体の描画（テクスチャ座標、頂点座標）
void HalfSphere::DrawObjectWithNormalMapping()
{
	// テクスチャ座標
	Vector3d t ;

	for (int v = 0 ; v < vDiv - 1 ; v ++)
	{
		int pNo = v ;

		glBegin(GL_QUAD_STRIP) ;

//			glNormal3fv(polygon[pNo].normal[2].v) ;
			t = SphericalVertex(-polygon[pNo].normal[2]) ;
//			t = SphericalVertex(polygon[pNo].normal[2]) ;
			glTexCoord2fv(t.v) ;
			glVertex3fv(polygon[pNo].vertex[2].v) ;

//			glNormal3fv(polygon[pNo].normal[3].v) ;
			t = SphericalVertex(-polygon[pNo].normal[3]) ;
//			t = SphericalVertex(polygon[pNo].normal[3]) ;
			glTexCoord2fv(t.v) ;
			glVertex3fv(polygon[pNo].vertex[3].v) ;

			for (int u = 0 ; u < uDiv ; u ++)
			{
//				glNormal3fv(polygon[pNo].normal[1].v) ;
				t = SphericalVertex(-polygon[pNo].normal[1]) ;
//				t = SphericalVertex(polygon[pNo].normal[1]) ;
				glTexCoord2fv(t.v) ;
				glVertex3fv(polygon[pNo].vertex[1].v) ;

//				glNormal3fv(polygon[pNo].normal[0].v) ;
				t = SphericalVertex(-polygon[pNo].normal[0]) ;
//				t = SphericalVertex(polygon[pNo].normal[0]) ;
				glTexCoord2fv(t.v) ;
				glVertex3fv(polygon[pNo].vertex[0].v) ;

				pNo += vDiv - 1 ;
			}

		glEnd() ;
	}
}

// 半球体の描画（テクスチャ座標、頂点座標）
// 屈折マッピング用
void HalfSphere::DrawObjectWithNormalMapping2()
{
	// テクスチャ座標
	Vector3d t ;

	for (int v = 0 ; v < vDiv - 1 ; v ++)
	{
		int pNo = v ;

		glBegin(GL_QUAD_STRIP) ;

//			glNormal3fv(polygon[pNo].normal[2].v) ;
//			t = SphericalVertex(-polygon[pNo].normal[2]) ;
			t = SphericalVertex(polygon[pNo].normal[2]) ;
			glTexCoord2fv(t.v) ;
			glVertex3fv(polygon[pNo].vertex[2].v) ;

//			glNormal3fv(polygon[pNo].normal[3].v) ;
//			t = SphericalVertex(-polygon[pNo].normal[3]) ;
			t = SphericalVertex(polygon[pNo].normal[3]) ;
			glTexCoord2fv(t.v) ;
			glVertex3fv(polygon[pNo].vertex[3].v) ;

			for (int u = 0 ; u < uDiv ; u ++)
			{
//				glNormal3fv(polygon[pNo].normal[1].v) ;
//				t = SphericalVertex(-polygon[pNo].normal[1]) ;
				t = SphericalVertex(polygon[pNo].normal[1]) ;
				glTexCoord2fv(t.v) ;
				glVertex3fv(polygon[pNo].vertex[1].v) ;

//				glNormal3fv(polygon[pNo].normal[0].v) ;
//				t = SphericalVertex(-polygon[pNo].normal[0]) ;
				t = SphericalVertex(polygon[pNo].normal[0]) ;
				glTexCoord2fv(t.v) ;
				glVertex3fv(polygon[pNo].vertex[0].v) ;

				pNo += vDiv - 1 ;
			}

		glEnd() ;
	}
}

// 半球体を直接描画
void HalfSphere::DrawPolyhedron(int divide, int vDivide, GLfloat r)
{
	HalfSphere obj(divide, vDivide, r,r,r) ;
	obj.DrawObject() ;
}

// 半球体を直接描画
// 反射マッピング用
void HalfSphere::DrawPolyhedronWithNormalMapping(int divide, int vDivide, GLfloat r)
{
	HalfSphere obj(divide, vDivide, r,r,r) ;
	obj.DrawObjectWithNormalMapping() ;
}

// 半球体を直接描画
// 屈折マッピング用
void HalfSphere::DrawPolyhedronWithNormalMapping2(int divide, int vDivide, GLfloat r)
{
	HalfSphere obj(divide, vDivide, r,r,r) ;
	obj.DrawObjectWithNormalMapping2() ;
}



GLfloat Icosahedron::X = 0.525731112119133606f, Icosahedron::Z = 0.850650808352039932f ;

GLfloat Icosahedron::i_v[][3] = { {  -Icosahedron::X, 0.0f,   Icosahedron::Z }, {   Icosahedron::X, 0.0f,   Icosahedron::Z }, {  -Icosahedron::X, 0.0f,  -Icosahedron::Z }, {   Icosahedron::X, 0.0f,  -Icosahedron::Z },
								 { 0.0f,   Icosahedron::Z,   Icosahedron::X }, { 0.0f,   Icosahedron::Z,  -Icosahedron::X }, { 0.0f,  -Icosahedron::Z,   Icosahedron::X }, { 0.0f,  -Icosahedron::Z,  -Icosahedron::X },
								 {   Icosahedron::Z,   Icosahedron::X, 0.0f }, {  -Icosahedron::Z,   Icosahedron::X, 0.0f }, {   Icosahedron::Z,  -Icosahedron::X, 0.0f }, {  -Icosahedron::Z,  -Icosahedron::X, 0.0f } } ;

int Icosahedron::i_o[][3] = { { 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 },{ 4, 8, 1 },
							  { 8,10, 1 }, { 8, 3,10 }, { 5, 3, 8 }, { 5, 2, 3 },{ 2, 7, 3 },
							  { 7,10, 3 }, { 7, 6,10 }, { 7,11, 6 }, {11, 0, 6 },{ 0, 1, 6 },
							  { 6, 1,10 }, { 9, 0,11 }, { 9,11, 2 }, { 9, 2, 5 },{ 7, 2,11 } } ;

GLfloat WineGlass::vertexes[N_VERTICIES][2] =
{
	{ 0.0f, 0.0f }, { 4.0f, 0.2f }, { 8.0f, 1.5f }, { 10.5f, 3.2f }, { 13.0f, 5.8f },
	{ 14.0f, 8.0f }, { 14.2f, 10.0f }, { 14.1f, 12.0f }, { 13.5f, 15.0f }, { 12.2f, 19.0f },
	{ 12.2f, 19.5f }, { 12.3f, 19.7f }, { 12.5f, 19.6f }, { 12.6f, 19.3f }, { 14.0f, 15.0f },
	{ 14.6f, 12.0f }, { 14.8f, 10.0f }, { 14.4f, 7.8f }, { 13.3f, 5.4f }, { 11.2f, 2.8f },
	{ 7.8f, 0.4f }, { 4.0f, -1.1f }, { 3.0f, -1.8f }, { 2.2f, -3.0f }, { 1.8f, -4.2f },
	{ 1.5f, -8.0f }, { 1.5f, -12.0f }, { 1.7f, -15.0f }, { 2.0f, -16.0f }, { 2.7f, -16.7f },
	{ 5.5f, -17.8f }, { 11.0f, -18.7f }, { 12.5f, -19.0f }, { 13.0f, -19.5f }, { 13.0f, -19.8f },
	{ 12.5f, -19.9f }, { 6.0f, -19.9f }, { 3.0f, -19.5f }, { 1.0f, -18.3f }, { 0.0f, -18.0f }
} ;

const Vector3d Vector3d::white(1.0f, 1.0f, 1.0f) ;


// from nVIDIA cube mapping sample code.
// simple teapot
static int patchdata[][16] =
{
    /* rim */
  {102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15},
    /* body */
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27},
  {24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40},
    /* lid */
  {96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101,
    101, 0, 1, 2, 3,},
  {0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117},
    /* bottom */
  {118, 118, 118, 118, 124, 122, 119, 121, 123, 126,
    125, 120, 40, 39, 38, 37},
    /* handle */
  {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
    53, 54, 55, 56},
  {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
    28, 65, 66, 67},
    /* spout */
  {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83},
  {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
    92, 93, 94, 95}
};


static float cpdata[][3] =
{
    {0.2, 0, 2.7}, {0.2, -0.112, 2.7}, {0.112, -0.2, 2.7}, {0,
    -0.2, 2.7}, {1.3375, 0, 2.53125}, {1.3375, -0.749, 2.53125},
    {0.749, -1.3375, 2.53125}, {0, -1.3375, 2.53125}, {1.4375,
    0, 2.53125}, {1.4375, -0.805, 2.53125}, {0.805, -1.4375,
    2.53125}, {0, -1.4375, 2.53125}, {1.5, 0, 2.4}, {1.5, -0.84,
    2.4}, {0.84, -1.5, 2.4}, {0, -1.5, 2.4}, {1.75, 0, 1.875},
    {1.75, -0.98, 1.875}, {0.98, -1.75, 1.875}, {0, -1.75,
    1.875}, {2, 0, 1.35}, {2, -1.12, 1.35}, {1.12, -2, 1.35},
    {0, -2, 1.35}, {2, 0, 0.9}, {2, -1.12, 0.9}, {1.12, -2,
    0.9}, {0, -2, 0.9}, {-2, 0, 0.9}, {2, 0, 0.45}, {2, -1.12,
    0.45}, {1.12, -2, 0.45}, {0, -2, 0.45}, {1.5, 0, 0.225},
    {1.5, -0.84, 0.225}, {0.84, -1.5, 0.225}, {0, -1.5, 0.225},
    {1.5, 0, 0.15}, {1.5, -0.84, 0.15}, {0.84, -1.5, 0.15}, {0,
    -1.5, 0.15}, {-1.6, 0, 2.025}, {-1.6, -0.3, 2.025}, {-1.5,
    -0.3, 2.25}, {-1.5, 0, 2.25}, {-2.3, 0, 2.025}, {-2.3, -0.3,
    2.025}, {-2.5, -0.3, 2.25}, {-2.5, 0, 2.25}, {-2.7, 0,
    2.025}, {-2.7, -0.3, 2.025}, {-3, -0.3, 2.25}, {-3, 0,
    2.25}, {-2.7, 0, 1.8}, {-2.7, -0.3, 1.8}, {-3, -0.3, 1.8},
    {-3, 0, 1.8}, {-2.7, 0, 1.575}, {-2.7, -0.3, 1.575}, {-3,
    -0.3, 1.35}, {-3, 0, 1.35}, {-2.5, 0, 1.125}, {-2.5, -0.3,
    1.125}, {-2.65, -0.3, 0.9375}, {-2.65, 0, 0.9375}, {-2,
    -0.3, 0.9}, {-1.9, -0.3, 0.6}, {-1.9, 0, 0.6}, {1.7, 0,
    1.425}, {1.7, -0.66, 1.425}, {1.7, -0.66, 0.6}, {1.7, 0,
    0.6}, {2.6, 0, 1.425}, {2.6, -0.66, 1.425}, {3.1, -0.66,
    0.825}, {3.1, 0, 0.825}, {2.3, 0, 2.1}, {2.3, -0.25, 2.1},
    {2.4, -0.25, 2.025}, {2.4, 0, 2.025}, {2.7, 0, 2.4}, {2.7,
    -0.25, 2.4}, {3.3, -0.25, 2.4}, {3.3, 0, 2.4}, {2.8, 0,
    2.475}, {2.8, -0.25, 2.475}, {3.525, -0.25, 2.49375},
    {3.525, 0, 2.49375}, {2.9, 0, 2.475}, {2.9, -0.15, 2.475},
    {3.45, -0.15, 2.5125}, {3.45, 0, 2.5125}, {2.8, 0, 2.4},
    {2.8, -0.15, 2.4}, {3.2, -0.15, 2.4}, {3.2, 0, 2.4}, {0, 0,
    3.15}, {0.8, 0, 3.15}, {0.8, -0.45, 3.15}, {0.45, -0.8,
    3.15}, {0, -0.8, 3.15}, {0, 0, 2.85}, {1.4, 0, 2.4}, {1.4,
    -0.784, 2.4}, {0.784, -1.4, 2.4}, {0, -1.4, 2.4}, {0.4, 0,
    2.55}, {0.4, -0.224, 2.55}, {0.224, -0.4, 2.55}, {0, -0.4,
    2.55}, {1.3, 0, 2.55}, {1.3, -0.728, 2.55}, {0.728, -1.3,
    2.55}, {0, -1.3, 2.55}, {1.3, 0, 2.4}, {1.3, -0.728, 2.4},
    {0.728, -1.3, 2.4}, {0, -1.3, 2.4}, {0, 0, 0}, {1.425,
    -0.798, 0}, {1.5, 0, 0.075}, {1.425, 0, 0}, {0.798, -1.425,
    0}, {0, -1.5, 0.075}, {0, -1.425, 0}, {1.5, -0.84, 0.075},
    {0.84, -1.5, 0.075}
};


// This "fastTeapot" routine is faster than glutSolidTeapot.
void
fastSolidTeapot(GLint grid, GLdouble scale)
{
  float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
  long i, j, k, l;

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_MAP2_VERTEX_3);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glScalef(0.5 * scale, 0.5 * scale, 0.5 * scale);
  glTranslatef(0.0, 0.0, -1.5);
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 3; l++) {
          p[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
          q[j][k][l] = cpdata[patchdata[i][j * 4 + (3 - k)]][l];
          if (l == 1)
            q[j][k][l] *= -1.0;
          if (i < 6) {
            r[j][k][l] =
              cpdata[patchdata[i][j * 4 + (3 - k)]][l];
            if (l == 0)
              r[j][k][l] *= -1.0;
            s[j][k][l] = cpdata[patchdata[i][j * 4 + k]][l];
            if (l == 0)
              s[j][k][l] *= -1.0;
            if (l == 1)
              s[j][k][l] *= -1.0;
          }
        }
      }
    }
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &p[0][0][0]);
    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &q[0][0][0]);
    glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    if (i < 6) {
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &r[0][0][0]);
      glEvalMesh2(GL_FILL, 0, grid, 0, grid);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &s[0][0][0]);
      glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    }
  }
}


#ifdef WIN32

// Win32 専用
// ディスプレイモードコントロールクラス

int DisplaySettingsCtrl::EnumAllSettings()
{
	return TRUE ;
}

DisplaySettingsCtrl::DisplaySettingsCtrl()
{
	nSettings = 0 ;
	devmode = NULL ;
}

DisplaySettingsCtrl::~DisplaySettingsCtrl()
{
}

#endif	// #ifdef WIN32

TextureObjectCtrl TextureObjectCtrl::textureObjectCtrl ;
