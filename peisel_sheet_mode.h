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
#include "peisel_state.h"
#include "peisel_io/peisel_save_as.h"

std::string SpritePathSplit(const std::string& line, const char& character, const unsigned int& section) {
	/*Split a line by a given character*/
	int counter = 0;
	int previousSectionIdx = 0;
	bool keepGoing = true;
	if (line.find(character) != std::string::npos) {
		while (keepGoing) {
			keepGoing = false;
			if (line.find(character, previousSectionIdx + 1) != std::string::npos) {
				if (counter == section) {
					if (counter > 0) {
						int startIdx = previousSectionIdx + 1;
						int endIdx = line.find(character, previousSectionIdx + 1);

						return line.substr(startIdx, endIdx - startIdx);
					}
					else {
						return line.substr(0, line.find(character));
					}
				}
				else {
					previousSectionIdx = line.find(character, previousSectionIdx + 1);
					counter++;
					keepGoing = true;
				}
			}
		}
		return "";
	}
	else {
		return line;
	}
}

void CreateSpriteSheet(pen::ui::Item* item, std::string fileName, bool confirmed) {
	/*Create the sprite sheet with the given paths*/
	if (confirmed) {
		PeiselState* peiselState = PeiselState::Get();
		std::vector<std::string> pathList;
		std::string path = "";
		std::string prevPath = "";
		bool keepGoing = true;
		int counter = 0;
		while (keepGoing) {
			path = SpritePathSplit(fileName, ';', counter);

			if (path == prevPath) {
				keepGoing = false;
				break;
			}

			if (path == "") {
				keepGoing = false;
				break;
			}
			
			counter++;

			/*Load in each path*/
			prevPath = path;
			peiselState->sprites.push_back(pen::CreateSprite(0, 0, 150, 75, path));
		}

		/*Create a square grid based on the number of sprites loaded*/
		int dimension = (int)pen::op::Ceil(pen::op::Sqrt((float)peiselState->sprites.size()));

		int pixelWidth = pen::PixelBufferWidth() / dimension;
		int pixelHeight = pen::PixelBufferHeight() / dimension;
		float scaleX = (float)peiselState->sprites[0]->width / (float)pixelWidth;
		float scaleY = (float)peiselState->sprites[0]->height / (float)pixelHeight;

		for (int j = 0; j < dimension; j++) {
			for (int i = 0; i < dimension; i++) {
				if (j * dimension + i > peiselState->sprites.size() - 1) {
					break;
					break;
				}

				pen::Scale(peiselState->sprites[j * dimension + i], scaleX, scaleY);
				peiselState->sprites[j * dimension + i]->x = i * pixelWidth;
				peiselState->sprites[j * dimension + i]->y = (dimension - j - 1) * pixelHeight;
				peiselState->sprites[j * dimension + i]->Draw();
			}
		}

		peiselState->spritesLoaded = true;
		peiselState->changes = true;

		pen::ui::RemoveItem(item);
		peiselState->spriteFileModal = nullptr;
		pen::ui::Submit();
	}
}

void OnSpriteSheetMode() {
	/*Grab a user directory and retrieve all .png files recursively*/
	if (!PeiselState::Get()->spritesLoaded && !PeiselState::Get()->spriteSelect) {
		pen::ui::Item* fileModal = pen::ui::AddItem(new pen::ui::FileModal(ID_ANY, pen::ui::FILE_INTENT::SELECT_FILES, ".png", pen::PEN_RED, pen::PEN_WHITE, nullptr, &CreateSpriteSheet));
		PeiselState::Get()->spriteFileModal = fileModal;
		pen::ui::Submit();
		PeiselState::Get()->spriteSelect = true;
	}
}