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

void LoadTileSprites(pen::ui::Item* item, std::string fileName, bool confirmed) {
	/*Load the tile sprites with the given paths*/
	if (confirmed) {
		PeiselState* peiselState = PeiselState::Get();
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

			pen::Item* tempItem = pen::CreateSprite(0, 0, 150, 75, path);

			/*Remove shell item now that image is loaded in memory*/
			pen::DeleteItem(tempItem);
			peiselState->selectableTileSprites.push_back(path);
		}
		peiselState->spritesLoaded = true;
		peiselState->selectedTileSlot = 0;
		peiselState->selectedTileSpriteName = peiselState->selectableTileSprites[0];
		peiselState->selectedTileSprite->UpdateText(PeiselState::Get()->selectedTileSpriteName);
		pen::ui::RemoveItem(item);
		peiselState->spriteFileModal = nullptr;
		pen::ui::Submit();
	}
}

std::string ParseTileSpritePath(std::string fileName, int length) {
	/*Parses the file name for displaying*/
	std::vector<std::string> pathList;
	std::string path = "";
	std::string prevPath = "";
	bool keepGoing = true;
	int counter = 0;

	if (fileName.find("C://") != std::string::npos) fileName = fileName.substr(4);

	while (keepGoing) {
		path = SpritePathSplit(fileName, '/', counter);

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

		pathList.push_back(path);
	}

	std::string pathName = pathList[pathList.size() - 1];
	if (pathName.length() > length) pathName = pathName.substr(0, length - 4) + "...";
	return pathName;
}

void PaintTiles() {
	/*Handles the click events in tile mode*/
	PeiselState* peiselState = PeiselState::Get();
	if (peiselState->spritesLoaded && peiselState->spriteSelect) {
		int x = 0;
		int y = 0;
		pen::Pen::GetMousePos(peiselState->mouseXPtr, peiselState->mouseYPtr);
		if (*peiselState->mouseXPtr >= 0.0f && *peiselState->mouseXPtr <= pen::Pen::ScreenWidth() && *peiselState->mouseYPtr >= 0.0f && *peiselState->mouseYPtr <= pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) {
			x = PeiselState::ScreenToPexelX(*peiselState->mouseXPtr);
			y = PeiselState::ScreenToPexelY(*peiselState->mouseYPtr);
		}

		if (x > 0) {
			if (optionList.data != nullptr && peiselState->action) {
				pen::ui::RemoveItem(optionList.data);
				pen::ui::Submit();
				optionList = { "", nullptr };
			}

			if (peiselState->tiles != nullptr) {
				/*Reallocate the tiles buffer if necessary*/
				bool update = false;
				if (peiselState->tileSize == TILES_10 && (int)peiselState->tiles[0] != 10) {
					delete[] peiselState->tiles;
					peiselState->tiles = new char[101];
					std::memset(peiselState->tiles, 0x00000001, 101);
					peiselState->tiles[0] = (char)10;
					update = true;
				}
				else if (peiselState->tileSize == TILES_50 && (int)peiselState->tiles[0] != 50) {
					delete[] peiselState->tiles;
					peiselState->tiles = new char[2501];
					std::memset(peiselState->tiles, 0x00000001, 2501);
					peiselState->tiles[0] = (char)50;
					update = true;
				}
				else if (peiselState->tileSize == TILES_100 && (int)peiselState->tiles[0] != 100) {
					delete[] peiselState->tiles;
					peiselState->tiles = new char[10001];
					std::memset(peiselState->tiles, 0x00000001, 10001);
					peiselState->tiles[0] = (char)100;
					update = true;
				}

				if (update) {
					for (int i = 0; i < peiselState->tileSprites.size(); i++) {
						pen::DeleteItem(peiselState->tileSprites[i].item);
					}
					peiselState->tileSprites.clear();
					for (int j = 0; j < peiselState->collisionBoxes.size(); j++) {
						pen::DeleteItem(peiselState->collisionBoxes[j].item);
					}
					peiselState->collisionBoxes.clear();
					peiselState->updateCanvas = true;
				}
			}
			else {
				peiselState->tiles = new char[101];
				std::memset(peiselState->tiles, 0x00000001, 101);
				peiselState->tiles[0] = (char)100;
			}

			/*Get the tile location*/
			int tileX = peiselState->tileSize * x / pen::PixelBufferWidth();
			int tileY = peiselState->tileSize * y / (pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN);
			int tileLocation = tileY * peiselState->tileSize + tileX;
			int replaceIdx = -1;

			if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::HELD) {
				/*Place or remove the selected sprite at that tile*/
				peiselState->penActive = true;
				peiselState->action = true;
				peiselState->changes = true;
				if (peiselState->tileAlt) {
					/*Remove the selected sprite*/
					std::vector<PeiselTileSprite> tempList;
					for (int i = 0; i < peiselState->tileSprites.size(); i++) {
						if (peiselState->tileSprites[i].tile == tileLocation) {
							replaceIdx = i;
						}
						else {
							tempList.push_back(peiselState->tileSprites[i]);
						}
					}
					if (replaceIdx != -1) {
						pen::DeleteItem(peiselState->tileSprites[replaceIdx].item);
						peiselState->tileSprites = tempList;
					}
				}
				else {
					/*Place the selected sprite*/
					pen::Item* sprite = pen::CreateSprite(pen::PixelBufferWidth() * tileX / peiselState->tileSize,
						(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize),
						pen::PixelBufferWidth() / peiselState->tileSize,
						(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize),
						peiselState->selectedTileSpriteName);
					pen::Scale(sprite, (pen::PixelBufferWidth() / peiselState->tileSize) / sprite->width, ((int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize)) / sprite->height);

					for (int i = 0; i < peiselState->tileSprites.size(); i++) {
						if (peiselState->tileSprites[i].tile == tileLocation) {
							replaceIdx = i;
							break;
						}
					}

					if (replaceIdx != -1) {
						peiselState->tileSprites[replaceIdx] = { sprite, tileLocation };
					}
					else {
						peiselState->tileSprites.push_back({ sprite, tileLocation });
					}
				}
				peiselState->updateCanvas = true;
			}
			else if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_RIGHT) == pen::in::KEYS::PRESSED) {
				/*Toggle a collision square on top of that tile*/
				peiselState->penActive = true;
				peiselState->action = true;
				peiselState->changes = true;
				for (int i = 0; i < peiselState->collisionBoxes.size(); i++) {
					if (peiselState->collisionBoxes[i].tile == tileLocation) {
						replaceIdx = i;
						break;
					}
				}

				if (replaceIdx != -1) {
					if (peiselState->tiles[tileLocation + 1] == (char) 1) {
						peiselState->tiles[tileLocation + 1] = (char) 2;
						peiselState->collisionBoxes[replaceIdx].item->color = pen::PEN_RED;
					}
					else {
						peiselState->tiles[tileLocation + 1] = (char) 1;
						peiselState->collisionBoxes[replaceIdx].item->color = pen::PEN_TRANSPARENT;
					}
				}
				else {
					pen::Item* sprite = pen::DrawRect(pen::PixelBufferWidth() * tileX / peiselState->tileSize,
						(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize),
						pen::PixelBufferWidth() / peiselState->tileSize,
						(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize),
						pen::PEN_WHITE,
						false);
					sprite->color = pen::PEN_RED;
					peiselState->collisionBoxes.push_back({ sprite, tileLocation });
					peiselState->tiles[tileLocation + 1] = (char) 2;
				}
				peiselState->updateCanvas = true;
			}
			else if ((pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::RELEASED) || (pen::Pen::MouseState(pen::in::KEYS::MOUSE_RIGHT) == pen::in::KEYS::RELEASED)
				|| (pen::Pen::KeyState(pen::in::KEYS::LEFT_SHIFT) == pen::in::KEYS::RELEASED) || (pen::Pen::KeyState(pen::in::KEYS::RIGHT_SHIFT) == pen::in::KEYS::RELEASED)) {
				peiselState->penActive = false;
				peiselState->action = false;
			}
		}
	}
}

void UpdateTileCanvas() {
	/*Redraws all of the buffer data*/
	PeiselState* peiselState = PeiselState::Get();
	if (peiselState->updateCanvas) {
		pen::Flush();
		for (int i = 0; i < peiselState->tileSprites.size(); i++) {
			peiselState->tileSprites[i].item->Draw();
		}
		for (int i = 0; i < peiselState->collisionBoxes.size(); i++) {
			peiselState->collisionBoxes[i].item->Draw();
		}
		peiselState->updateCanvas = false;
	}

	/*Draw the current tile*/
	int x = 0;
	int y = 0;
	if (*peiselState->mouseXPtr >= 0.0f && *peiselState->mouseXPtr <= pen::Pen::ScreenWidth() && *peiselState->mouseYPtr >= 0.0f && *peiselState->mouseYPtr <= pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) {
		x = PeiselState::ScreenToPexelX(*peiselState->mouseXPtr);
		y = PeiselState::ScreenToPexelY(*peiselState->mouseYPtr);
	}

	if (x > 0) {
		int tileX = peiselState->tileSize * x / pen::PixelBufferWidth();
		int tileY = peiselState->tileSize * y / (pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN);
		int tileLocation = tileY * peiselState->tileSize + tileX;
		int width = pen::PixelBufferWidth() / peiselState->tileSize;
		int height = (int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize);

		if (peiselState->currentTile != nullptr) {
			if (peiselState->currentTile->width != width) pen::Scale(peiselState->currentTile, (1.0f / (float)peiselState->tileSize) / peiselState->currentTile->width, 1.0f);
			if (peiselState->currentTile->height != height) pen::Scale(peiselState->currentTile, 1.0f, (1.0f / (float)peiselState->tileSize) / peiselState->currentTile->height);

			peiselState->currentTile->x = pen::PixelBufferWidth() * tileX / peiselState->tileSize;
			peiselState->currentTile->y = (int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize);
		}
		else {
			peiselState->currentTile = pen::DrawRect(pen::PixelBufferWidth() * tileX / peiselState->tileSize,
				(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize),
				pen::PixelBufferWidth() / peiselState->tileSize,
				(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize),
				pen::PEN_GREEN,
				false);
		}
		peiselState->currentTile->Draw();
	}
}

void TileMode() {
	/*Continues an ongoing event in tile mode*/
	if (!PeiselState::Get()->spritesLoaded && !PeiselState::Get()->spriteSelect) {
		pen::ui::Item* fileModal = pen::ui::AddItem(new pen::ui::FileModal(ID_ANY, pen::ui::FILE_INTENT::SELECT_FILES, ".png", pen::PEN_RED, pen::PEN_WHITE, nullptr, &LoadTileSprites));
		PeiselState::Get()->spriteFileModal = fileModal;
		pen::ui::Submit();
		PeiselState::Get()->spriteSelect = true;
	}
	else {
		if (PeiselState::Get()->penActive) {
			PeiselState* peiselState = PeiselState::Get();
			pen::Pen::GetMousePos(peiselState->mouseXPtr, peiselState->mouseYPtr);
			int x = PeiselState::ScreenToPexelX(*peiselState->mouseXPtr);
			int y = PeiselState::ScreenToPexelY(*peiselState->mouseYPtr);

			if (x > 0) {
				/*Get the tile location*/
				int tileX = peiselState->tileSize * x / pen::PixelBufferWidth();
				int tileY = peiselState->tileSize * y / (pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN);
				int tileLocation = tileY * peiselState->tileSize + tileX;
				int replaceIdx = -1;

				if (((pen::Pen::KeyState(pen::in::KEYS::LEFT_SHIFT) == pen::in::KEYS::HELD)
					|| (pen::Pen::KeyState(pen::in::KEYS::RIGHT_SHIFT) == pen::in::KEYS::HELD))
					&& (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::HELD)) {
					/*Remove the selected sprite at that tile*/
					std::vector<PeiselTileSprite> tempList;
					for (int i = 0; i < peiselState->tileSprites.size(); i++) {
						if (peiselState->tileSprites[i].tile == tileLocation) {
							replaceIdx = i;
						}
						else {
							tempList.push_back(peiselState->tileSprites[i]);
						}
					}
					if (replaceIdx != -1) {
						pen::DeleteItem(peiselState->tileSprites[replaceIdx].item);
						peiselState->tileSprites = tempList;
					}
					peiselState->updateCanvas = true;
				}
				else if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::HELD) {
					/*Place or remove the selected sprite at that tile*/
					if (peiselState->tileAlt) {
						/*Remove the selected sprite*/
						std::vector<PeiselTileSprite> tempList;
						for (int i = 0; i < peiselState->tileSprites.size(); i++) {
							if (peiselState->tileSprites[i].tile == tileLocation) {
								replaceIdx = i;
							}
							else {
								tempList.push_back(peiselState->tileSprites[i]);
							}
						}
						if (replaceIdx != -1) {
							pen::DeleteItem(peiselState->tileSprites[replaceIdx].item);
							peiselState->tileSprites = tempList;
						}
					}
					else {
						/*Place the selected sprite*/
						pen::Item* sprite = pen::CreateSprite(pen::PixelBufferWidth() * tileX / peiselState->tileSize,
							(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize),
							pen::PixelBufferWidth() / peiselState->tileSize,
							(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize),
							peiselState->selectedTileSpriteName);
						pen::Scale(sprite, (pen::PixelBufferWidth() / peiselState->tileSize) / sprite->width, ((int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize)) / sprite->height);

						for (int i = 0; i < peiselState->tileSprites.size(); i++) {
							if (peiselState->tileSprites[i].tile == tileLocation) {
								replaceIdx = i;
								break;
							}
						}

						if (replaceIdx != -1) {
							peiselState->tileSprites[replaceIdx] = { sprite, tileLocation };
						}
						else {
							peiselState->tileSprites.push_back({ sprite, tileLocation });
						}
					}
					peiselState->updateCanvas = true;
				}
				else if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_RIGHT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_RIGHT) == pen::in::KEYS::HELD) {
					/*Toggle a collision square on top of that tile*/
					for (int i = 0; i < peiselState->collisionBoxes.size(); i++) {
						if (peiselState->collisionBoxes[i].tile == tileLocation) {
							replaceIdx = i;
							break;
						}
					}

					if (replaceIdx != -1) {
						if (peiselState->tiles[tileLocation + 1] == (char) 1) {
							peiselState->tiles[tileLocation + 1] = (char) 2;
							peiselState->collisionBoxes[replaceIdx].item->color = pen::PEN_RED;
						}
						else {
							peiselState->tiles[tileLocation + 1] = (char) 1;
							peiselState->collisionBoxes[replaceIdx].item->color = pen::PEN_TRANSPARENT;
						}
					}
					else {
						pen::Item* sprite = pen::DrawRect(pen::PixelBufferWidth() * tileX / peiselState->tileSize,
							(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) * tileY / peiselState->tileSize),
							pen::PixelBufferWidth() / peiselState->tileSize,
							(int)((pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) / peiselState->tileSize),
							pen::PEN_WHITE,
							false);
						sprite->color = pen::PEN_RED;
						peiselState->collisionBoxes.push_back({ sprite, tileLocation });
						peiselState->tiles[tileLocation + 1] = (char) 2;
					}
					peiselState->updateCanvas = true;
				}
			}
		}
	}
}