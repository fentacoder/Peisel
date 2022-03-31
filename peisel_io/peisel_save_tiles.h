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
#include "../peisel_state.h"

void PrepareTilesPixelBuffer() {
	PeiselState* peiselState = PeiselState::Get();
	pen::Flush();
	for (int i = 0; i < peiselState->tileSprites.size(); i++) {
		peiselState->tileSprites[i].item->Draw();
	}
}

void SaveTiles(pen::ui::Item* item, std::string fileName, bool confirmed) {
	/*Save the tile information*/
	if (confirmed) {
		PeiselState* peiselState = PeiselState::Get();
		if (peiselState->tiles != nullptr) {
			fileName = fileName.substr(0, fileName.length() - 4) + ".pentm";

			/*The first char of the buffer is the number of tiles*/
			std::ofstream tileFile(fileName, std::ios::out | std::ios::binary);
			if (tileFile.is_open()) {
				int tileCount = ((int)peiselState->tiles[0] * (int)peiselState->tiles[0]) + 1;
				for (int i = 0; i < tileCount; i++) {
					tileFile.write((char*)&peiselState->tiles[i], sizeof(char));
				}
				tileFile.close();
			}
		}
	}
}

void LoadTiles(std::string fileName) {
	/*Load the tile information*/
	PeiselState* peiselState = PeiselState::Get();
	if (peiselState->tiles != nullptr) {
		delete[] peiselState->tiles;
		peiselState->tiles = nullptr;
	}

	if(fileName.find(".") != std::string::npos) fileName = fileName.substr(0, fileName.length() - 4) + ".pentm";

	std::ifstream tileFile(fileName, std::ios::out | std::ios::binary);
	if (tileFile.is_open()) {
		char tileCountValue = 1000001;
		int tileCount = 1;
		for (int i = 0; i < tileCount; i++) {
			if (i == 0) {
				tileFile.read((char*)&tileCountValue, sizeof(char));
				tileCount = (int)tileCountValue * (int)tileCountValue;
				peiselState->tiles = new char[tileCount];
				peiselState->tiles[0] = tileCountValue;
			}
			else {
				tileFile.read((char*)&peiselState->tiles[i], sizeof(char));
			}
		}
		tileFile.close();
	}
}