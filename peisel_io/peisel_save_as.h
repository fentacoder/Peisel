/*************************************************************************************************
 Copyright 2022 Jamar Phillip

Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*************************************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <png.h>
#include "../peisel_state.h"
#include "peisel_save_tiles.h"

bool PeiselExit(pen::ui::Item* item, int button, int action) {
	/*Terminates the program*/
	if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
		glfwTerminate();
	}
	return true;
}

void SavePng(pen::ui::Item* item, std::string fileName, bool confirmed) {
	/*Save pixel buffer as a png*/
	if (confirmed) {
		FILE* fp;
		fopen_s(&fp, fileName.c_str(), "wb");
		png_voidp userErrorPtr = nullptr;
		png_error_ptr userErrorFn = nullptr;
		png_error_ptr userWarningFn = nullptr;

		if (fp) {
			png_structp pngPtr = png_create_write_struct
			(PNG_LIBPNG_VER_STRING, (png_voidp)userErrorPtr,
				userErrorFn, userWarningFn);

			if (pngPtr) {

				png_infop infoPtr = png_create_info_struct(pngPtr);
				if (!infoPtr)
				{
					png_destroy_write_struct(&pngPtr,
						(png_infopp)NULL);
				}

				if (setjmp(png_jmpbuf(pngPtr)))
				{
					png_destroy_write_struct(&pngPtr, &infoPtr);
					fclose(fp);
				}

				png_init_io(pngPtr, fp);

				png_set_IHDR(pngPtr, infoPtr, pen::PixelBufferWidth(), pen::PixelBufferHeight(),
					8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

				unsigned char* pixels = pen::PixelBuffer();

				/*Converts pixel buffer to row_pointers*/
				png_byte** rowPointers = new png_byte * [pen::PixelBufferHeight()];

				int rowPos = (pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4) - (pen::PixelBufferWidth() * 4);
				for (int y = 0; y < pen::PixelBufferHeight(); y++) {
					rowPointers[y] = (png_byte*)(&pixels[rowPos]);
					rowPos -= pen::PixelBufferWidth() * 4;
				}


				png_set_rows(pngPtr, infoPtr, rowPointers);

				png_write_png(pngPtr, infoPtr, PNG_TRANSFORM_IDENTITY, NULL);

				png_write_end(pngPtr, infoPtr);
				png_destroy_write_struct(&pngPtr, &infoPtr);

				delete[] rowPointers;
			}

			fclose(fp);

			if (item != nullptr) {
				if (optionList.data != nullptr) {
					pen::ui::RemoveItem(optionList.data);
					optionList = { "", nullptr };
				}
				pen::ui::RemoveItem(item);	
			}
		}
	}
}

void SaveSelect(pen::ui::Item* item, std::string fileName, bool confirmed) {
	if (confirmed) {
		switch (PeiselState::Get()->appMode) {
		case PEISEL_DRAW:
		case PEISEL_SHEET:
			SavePng(item, fileName, confirmed);
			PeiselState::Get()->outputFile = fileName;
			PeiselState::Get()->changes = false;
			PeiselState::Get()->saveBox->SetColor(pen::PEN_GREEN);
			pen::ui::Submit();
			break;
		case PEISEL_TILES:
			PrepareTilesPixelBuffer();
			/*File modal gets removed in SavePng so no need to pass it in to SaveTiles*/
			SavePng(item, fileName, confirmed);
			SaveTiles(nullptr, fileName, confirmed);
			PeiselState::Get()->outputFile = fileName;
			PeiselState::Get()->changes = false;
			PeiselState::Get()->saveBox->SetColor(pen::PEN_GREEN);
			pen::ui::Submit();
			break;
		default:
			break;
		}
	}
}

bool SaveAs(pen::ui::Item* item, int button, int action) {
	/*Save pixel buffer as a png*/
	if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
		if (PeiselState::Get()->outputFile == "") {
			/*Grab the user specified file path*/
			pen::ui::Item* fileModal = pen::ui::AddItem(new pen::ui::FileModal(ID_ANY, pen::ui::FILE_INTENT::SAVE_AS, ".png", pen::PEN_RED, pen::PEN_WHITE, nullptr, &SaveSelect));
			pen::ui::Submit();
		}
		else {
			switch (PeiselState::Get()->appMode) {
			case PEISEL_DRAW:
			case PEISEL_SHEET:
				SavePng(nullptr, PeiselState::Get()->outputFile, true);
				PeiselState::Get()->changes = false;
				PeiselState::Get()->saveBox->SetColor(pen::PEN_GREEN);
				pen::ui::Submit();
				break;
			case PEISEL_TILES:
				PrepareTilesPixelBuffer();
				SavePng(nullptr, PeiselState::Get()->outputFile, true);
				SaveTiles(nullptr, PeiselState::Get()->outputFile, true);
				PeiselState::Get()->changes = false;
				PeiselState::Get()->saveBox->SetColor(pen::PEN_GREEN);
				pen::ui::Submit();
				break;
			default:
				break;
			}
		}
	}
	return true;
}