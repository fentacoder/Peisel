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

bool NavClick(pen::ui::Item* item, int button, int action);

bool SelectMode(pen::ui::Item* item, int button, int action) {
    /*Selects the fill tool*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        PeiselState* peiselState = PeiselState::Get();
        peiselState->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);

        if (item->origText == "Drawing" && peiselState->appMode != PEISEL_DRAW) {
            /*Draw mode for creating sprites*/
            peiselState->tool = PEISEL_BRUSH;
            peiselState->appMode = PEISEL_DRAW;
            peiselState->outputFile = "";
            peiselState->changes = false;
            peiselState->currentColorBox->AllowActive(true);
            peiselState->saveBox->SetColor(pen::PEN_RED);
            std::memset(pen::PixelBuffer(), 0, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);

            peiselState->navBar->childItems[1]->AllowActive(true);
            peiselState->navBar->childItems[2]->AllowActive(true);
            peiselState->navBar->childItems[3]->AllowActive(true);
            peiselState->navBar->childItems[4]->AllowActive(true);

            if (peiselState->spriteFileModal != nullptr) {
                pen::ui::RemoveItem(peiselState->spriteFileModal);
                peiselState->spriteFileModal = nullptr;
            }

            if (peiselState->selectedTileSprite != nullptr) {
                pen::ui::RemoveItem(peiselState->selectedTileSprite);
                peiselState->selectedTileSprite = nullptr;
            }
        }
        else if (item->origText == "Sprite Sheet" && peiselState->appMode != PEISEL_SHEET) {
            /*Sprite sheet mode for combining sprites into one png*/
            peiselState->tool = PEISEL_COMBINER;
            peiselState->appMode = PEISEL_SHEET;
            peiselState->outputFile = "";
            peiselState->changes = false;
            peiselState->currentColorBox->AllowActive(false);
            std::memset(pen::PixelBuffer(), 0, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);
            peiselState->navBar->childItems[1]->AllowActive(false);
            peiselState->navBar->childItems[2]->AllowActive(false);
            peiselState->navBar->childItems[3]->AllowActive(false);
            peiselState->navBar->childItems[4]->AllowActive(false);

            if (peiselState->spriteFileModal != nullptr) {
                pen::ui::RemoveItem(peiselState->spriteFileModal);
                peiselState->spriteFileModal = nullptr;
            }

            if (peiselState->selectedTileSprite != nullptr) {
                pen::ui::RemoveItem(peiselState->selectedTileSprite);
                peiselState->selectedTileSprite = nullptr;
            }
        }
        else if (item->origText == "Tile Map" && peiselState->appMode != PEISEL_TILES) {
            peiselState->tool = PEISEL_BRUSH;
            peiselState->appMode = PEISEL_TILES;
            peiselState->outputFile = "";
            peiselState->changes = false;
            peiselState->currentColorBox->AllowActive(false);
            std::memset(pen::PixelBuffer(), 0, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);
            peiselState->navBar->childItems[1]->AllowActive(false);
            peiselState->navBar->childItems[2]->AllowActive(false);
            peiselState->navBar->childItems[3]->AllowActive(false);
            peiselState->navBar->childItems[4]->AllowActive(true);

            if (peiselState->spriteFileModal != nullptr) {
                pen::ui::RemoveItem(peiselState->spriteFileModal);
                peiselState->spriteFileModal = nullptr;
            }

            if (peiselState->selectedTileSprite == nullptr) {
                peiselState->selectedTileSprite = pen::ui::AddItem(new pen::ui::TextBox(ID_ANY, "",
                    pen::Vec3(pen::Pen::ScreenWidth() / 5.0f * 3.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f),
                    30, pen::PEN_TRANSPARENT, pen::PEN_BLACK, nullptr, nullptr, true));
            }
        }
        peiselState->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);
        peiselState->saveBox->SetColor(pen::PEN_RED);
        peiselState->spritesLoaded = false;
        peiselState->spriteSelect = false;
        pen::ui::RemoveItem(optionList.data);
        optionList = { "", nullptr };
        pen::ui::Submit();
    }
    return true;
}

bool SelectFillTool(pen::ui::Item* item, int button, int action) {
    /*Selects the fill tool*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        PeiselState::Get()->tool = PEISEL_FILL;
        PeiselState::Get()->currentColorBox->SetColor(pen::PEN_CYAN);
        pen::ui::Submit();
    }
    return true;
}

bool SelectColor(pen::ui::Item* item, int button, int action) {
    /*Updates color of brush*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        if (item->origText == "Light Gray") {
            PeiselState::Get()->currentColor = pen::PEN_LIGHT_GRAY;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_LIGHT_GRAY);
        }
        else if (item->origText == "Gray") {
            PeiselState::Get()->currentColor = pen::PEN_GRAY;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_GRAY);
        }
        else if (item->origText == "Red") {
            PeiselState::Get()->currentColor = pen::PEN_RED;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_RED);
        }
        else if (item->origText == "Blue") {
            PeiselState::Get()->currentColor = pen::PEN_BLUE;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_BLUE);
        }
        else if (item->origText == "Green") {
            PeiselState::Get()->currentColor = pen::PEN_GREEN;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_GREEN);
        }
        else if (item->origText == "Dark Green") {
            PeiselState::Get()->currentColor = pen::PEN_DARK_GREEN;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_DARK_GREEN);
        }
        else if (item->origText == "Cyan") {
            PeiselState::Get()->currentColor = pen::PEN_CYAN;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_CYAN);
        }
        else if (item->origText == "Violet") {
            PeiselState::Get()->currentColor = pen::PEN_VIOLET;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_VIOLET);
        }
        else if (item->origText == "Purple") {
            PeiselState::Get()->currentColor = pen::PEN_PURPLE;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_PURPLE);
        }
        else if (item->origText == "Yellow") {
            PeiselState::Get()->currentColor = pen::PEN_YELLOW;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_YELLOW);
        }
        else if (item->origText == "Orange") {
            PeiselState::Get()->currentColor = pen::PEN_ORANGE;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_ORANGE);
        }
        else if (item->origText == "Brown") {
            PeiselState::Get()->currentColor = pen::PEN_BROWN;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_BROWN);
        }
        else if (item->origText == "Beige") {
            PeiselState::Get()->currentColor = pen::PEN_BEIGE;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_BEIGE);
        }
        else if (item->origText == "Gold") {
            PeiselState::Get()->currentColor = pen::PEN_GOLD;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_GOLD);
        }
        else if (item->origText == "White") {
            PeiselState::Get()->currentColor = pen::PEN_WHITE;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_WHITE);
        }
        else if (item->origText == "Black") {
            PeiselState::Get()->currentColor = pen::PEN_BLACK;
            PeiselState::Get()->currentColorBox->childItems[0]->SetColor(pen::PEN_BLACK);
        }
        pen::ui::Submit();
    }
    return true;
}

bool SelectShape(pen::ui::Item* item, int button, int action) {
    /*Updates current shape selected*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        PeiselState::Get()->appState = DRAW_SHAPE;
        if (item->origText == "Line") {
            PeiselState::Get()->currentShape = DRAW_LINE;
        }
        else if (item->origText == "Triangle") {
            PeiselState::Get()->currentShape = DRAW_TRIANGLE;
        }
        else if (item->origText == "Rectangle") {
            PeiselState::Get()->currentShape = DRAW_RECTANGLE;
        }
        else if (item->origText == "Circle") {
            PeiselState::Get()->currentShape = DRAW_CIRCLE;
        }
        PeiselState::Get()->currentColorBox->SetColor(pen::PEN_RED);
        PeiselState::Get()->tool = PEISEL_BRUSH;
        pen::ui::Submit();
    }
    return true;
}

bool SelectBrushSize(pen::ui::Item* item, int button, int action) {
    /*Updates the brush size*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        PeiselState::Get()->appState = DRAW_BRUSHING;
        if (PeiselState::Get()->appMode == PEISEL_TILES) {
            if (item->origText == "10 x 10") {
                PeiselState::Get()->tileSize = TILES_10;
                PeiselState::Get()->tool = PEISEL_BRUSH;
                PeiselState::Get()->changes = true;
            }
            else if (item->origText == "50 x 50") {
                PeiselState::Get()->tileSize = TILES_50;
                PeiselState::Get()->tool = PEISEL_BRUSH;
                PeiselState::Get()->changes = true;
            }
            else if (item->origText == "100 x 100") {
                PeiselState::Get()->tileSize = TILES_100;
                PeiselState::Get()->tool = PEISEL_BRUSH;
                PeiselState::Get()->changes = true;
            }
        }
        else {
            if (item->origText == "Small") {
                PeiselState::Get()->brushSize = BRUSH_SMALL;
                PeiselState::Get()->tool = PEISEL_BRUSH;
            }
            else if (item->origText == "Medium") {
                PeiselState::Get()->brushSize = BRUSH_MEDIUM;
                PeiselState::Get()->tool = PEISEL_BRUSH;
            }
            else if (item->origText == "Large") {
                PeiselState::Get()->brushSize = BRUSH_LARGE;
                PeiselState::Get()->tool = PEISEL_BRUSH;
            }
        }
        PeiselState::Get()->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);
        pen::ui::Submit();
    }
    return true;
}

bool NavClick(pen::ui::Item* item, int button, int action) {
    /*Nav button item has been clicked*/
    if (button == pen::in::KEYS::MOUSE_LEFT && action == pen::in::KEYS::PRESSED) {
        if (item->origText == "File") {
            /*Open file drop down*/
            if (optionList.name != "File") {
                pen::ui::RemoveItem(optionList.data);
                pen::ui::Item* fileList = pen::ui::AddItem(new pen::ui::VerticalList(ID_ANY,
                    pen::Vec3(PeiselState::Get()->navBar->childItems[0]->positions.x, PeiselState::Get()->navBar->childItems[0]->positions.y - pen::Pen::ScreenHeight() / 3.0f, 0.0f),
                    pen::Vec2(pen::Pen::ScreenWidth() / 5.0f, pen::Pen::ScreenHeight() / 3.0f), pen::PEN_LIGHT_GRAY, pen::PEN_GRAY, nullptr, nullptr, "File"));
                fileList->Push(new pen::ui::Button(ID_ANY, (PeiselState::Get()->outputFile.length() > 0 ? "Save" : "Save As"), pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, fileList, &SaveAs, true));
                fileList->Push(new pen::ui::Button(ID_ANY, "Exit", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, fileList, &PeiselExit, true));
                optionList = { "File", fileList };
                pen::ui::Submit();
            }
        }
        else if (item->origText == "Tools") {
            /*Open tools drop down, this will have the colors and the shapes*/
            if (optionList.name != "Tools") {
                pen::ui::RemoveItem(optionList.data);
                pen::ui::Item* toolsList = pen::ui::AddItem(new pen::ui::VerticalList(ID_ANY,
                    pen::Vec3(PeiselState::Get()->navBar->childItems[1]->positions.x, PeiselState::Get()->navBar->childItems[1]->positions.y - pen::Pen::ScreenHeight() / 3.0f, 0.0f),
                    pen::Vec2(pen::Pen::ScreenWidth() / 5.0f, pen::Pen::ScreenHeight() / 3.0f), pen::PEN_LIGHT_GRAY, pen::PEN_GRAY, nullptr, nullptr, "Tools"));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Fill", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectFillTool, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Light Gray", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 10, pen::PEN_TRANSPARENT, pen::PEN_GRAY, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Gray", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Red", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_RED, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Blue", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLUE, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Green", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GREEN, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Dark Green", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 10, pen::PEN_TRANSPARENT, pen::PEN_DARK_GREEN, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Cyan", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_CYAN, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Violet", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_VIOLET, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Purple", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_PURPLE, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Yellow", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_YELLOW, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Orange", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_ORANGE, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Brown", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BROWN, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Beige", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BEIGE, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Gold", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GOLD, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "White", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_WHITE, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Black", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectColor, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Line", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectShape, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Triangle", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectShape, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Rectangle", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectShape, true));
                toolsList->Push(new pen::ui::Button(ID_ANY, "Circle", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_BLACK, toolsList, &SelectShape, true));
                optionList = { "Tools", toolsList };
                pen::ui::Submit();
            }
        }
        else if (item->origText == "Draw") {
            /*Select brush tool*/
            PeiselState::Get()->tool = PEISEL_BRUSH;
            PeiselState::Get()->currentColor = pen::PEN_BLACK;
            PeiselState::Get()->appState = DRAW_BRUSHING;
            PeiselState::Get()->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);
            pen::ui::Submit();
        }
        else if (item->origText == "Erase") {
            /*Select eraser tool*/
            PeiselState::Get()->tool = PEISEL_ERASER;
            PeiselState::Get()->currentColor = pen::PEN_TRANSPARENT;
            PeiselState::Get()->appState = DRAW_BRUSHING;
            PeiselState::Get()->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);
            pen::ui::Submit();
        }
        else if (item->origText == "Size") {
            /*Open size drop down*/
            if (optionList.name != "Size") {
                pen::ui::RemoveItem(optionList.data);
                pen::ui::Item* sizeList = pen::ui::AddItem(new pen::ui::VerticalList(ID_ANY,
                    pen::Vec3(PeiselState::Get()->navBar->childItems[4]->positions.x, PeiselState::Get()->navBar->childItems[4]->positions.y - pen::Pen::ScreenHeight() / 3.0f, 0.0f),
                    pen::Vec2(pen::Pen::ScreenWidth() / 5.0f, pen::Pen::ScreenHeight() / 3.0f), pen::PEN_LIGHT_GRAY, pen::PEN_GRAY, nullptr, nullptr, "Size"));
                if (PeiselState::Get()->appMode == PEISEL_TILES) {
                    sizeList->Push(new pen::ui::Button(ID_ANY, "10 x 10", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                    sizeList->Push(new pen::ui::Button(ID_ANY, "50 x 50", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                    sizeList->Push(new pen::ui::Button(ID_ANY, "100 x 100", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                }
                else {
                    sizeList->Push(new pen::ui::Button(ID_ANY, "Small", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                    sizeList->Push(new pen::ui::Button(ID_ANY, "Medium", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                    sizeList->Push(new pen::ui::Button(ID_ANY, "Large", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, sizeList, &SelectBrushSize, true));
                }
                
                optionList = { "Size", sizeList };
                pen::ui::Submit();
            }
        }
        else if (item->origText == "Mode") {
            /*Open mode drop down*/
            if (optionList.name != "Mode") {
                pen::ui::RemoveItem(optionList.data);
                pen::ui::Item* modeList = pen::ui::AddItem(new pen::ui::VerticalList(ID_ANY,
                    pen::Vec3(PeiselState::Get()->navBar->childItems[PeiselState::Get()->navBar->childItems.size() - 1]->positions.x, PeiselState::Get()->navBar->childItems[PeiselState::Get()->navBar->childItems.size() - 1]->positions.y - pen::Pen::ScreenHeight() / 3.0f, 0.0f),
                    pen::Vec2(pen::Pen::ScreenWidth() / 5.0f, pen::Pen::ScreenHeight() / 3.0f), pen::PEN_LIGHT_GRAY, pen::PEN_GRAY, nullptr, nullptr, "Mode"));
                modeList->Push(new pen::ui::Button(ID_ANY, "Drawing", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, modeList, &SelectMode, true));
                modeList->Push(new pen::ui::Button(ID_ANY, "Sprite Sheet", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 12, pen::PEN_TRANSPARENT, pen::PEN_GRAY, modeList, &SelectMode, true));
                modeList->Push(new pen::ui::Button(ID_ANY, "Tile Map", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 9, pen::PEN_TRANSPARENT, pen::PEN_GRAY, modeList, &SelectMode, true));
                optionList = { "Mode", modeList };
                pen::ui::Submit();
            }
        }
    }
    return true;
}