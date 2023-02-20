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
#include "nav_bar.h"
#include "draw_mode.h"
#include "peisel_sheet_mode.h"
#include "tile_mode.h"

PeiselState* PeiselState::instance = nullptr;

bool Paint() {
    switch (PeiselState::Get()->appMode) {
    case PEISEL_DRAW:
        PaintDraw();
        break;
    case PEISEL_TILES:
        PaintTiles();
        break;
    default:
        break;
    }
    return true;
}

class App : public pen::Pen {
public:
    

public:
    void CheckSave() {
        if(PeiselState::Get()->changes) {
            if (PeiselState::Get()->tileAlt) {
                if (PeiselState::Get()->saveBox->color.z != pen::PEN_BLUE.z) {
                    PeiselState::Get()->saveBox->SetColor(pen::PEN_BLUE);
                    pen::ui::Submit();
                }
            }
            else {
                if (PeiselState::Get()->saveBox->color.x != pen::PEN_RED.x) {
                    PeiselState::Get()->saveBox->SetColor(pen::PEN_RED);
                    pen::ui::Submit();
                }
            }
        }
    }

    void HandleUpdates() {
        CheckSave();
        if (*PeiselState::Get()->mouseXPtr >= 0.0f && *PeiselState::Get()->mouseXPtr <= pen::PixelBufferWidth() && *PeiselState::Get()->mouseYPtr >= 0.0f && *PeiselState::Get()->mouseYPtr <= pen::PixelBufferHeight() - CANVAS_TOP_MARGIN) PeiselState::Get()->penActive = false;
        switch (PeiselState::Get()->appMode) {
        case PEISEL_DRAW:
            DrawMode();
            UpdateDrawCanvas();
            break;
        case PEISEL_SHEET:
            OnSpriteSheetMode();
            break;
        case PEISEL_TILES:
            TileMode();
            UpdateTileCanvas();
            break;
        default:
            break;
        }
    }

    void OnCreate() override {
        pen::ui::Item* pixelBuffer = pen::ui::LM::pixelLayer->layerItems[0];
        pixelBuffer->GetSize()->y = pixelBuffer->GetSize()->y - CANVAS_TOP_MARGIN;

        /*Allocates a buffer for free drawing*/
        PeiselState* peiselState = PeiselState::Get();
        peiselState->freeDrawBuffer = new unsigned char[pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4];
        std::memset(peiselState->freeDrawBuffer, 0, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);

        optionList = { "", nullptr };
        peiselState->navBar = pen::ui::AddItem(new pen::ui::NavBar(PEISEL_NAVBAR_ID, CANVAS_TOP_MARGIN, pen::PEN_LIGHT_GRAY));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "File", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Tools", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 5, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Draw", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Erase", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 5, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Size", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Mode", pen::Vec3(0.0f, pen::PixelBufferHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick));

        peiselState->currentColorBox = pen::ui::AddItem(new pen::ui::Item(PEISEL_CURRENT_COLOR_BOX_ID, 
            pen::Vec3(pen::PixelBufferWidth() / 10.0f * 9.0f, pen::PixelBufferHeight() - CANVAS_TOP_MARGIN, 0.0f), pen::Vec2(pen::PixelBufferWidth() / 10.0f - 10.0f, CANVAS_TOP_MARGIN),
            pen::ui::Shape::QUAD, pen::PEN_LIGHT_GRAY, nullptr, nullptr, true));
        peiselState->currentColorBox->Push(new pen::ui::Item(ID_ANY, 
            pen::Vec3(peiselState->currentColorBox->GetPosition()->x + 4.0f, peiselState->currentColorBox->GetPosition()->y + 4.0f, 0.0f),
            pen::Vec2(peiselState->currentColorBox->GetSize()->x - 8.0f, peiselState->currentColorBox->GetSize()->y - 8.0f),
            pen::ui::Shape::QUAD, pen::PEN_BLACK, peiselState->currentColorBox, nullptr, true));

        peiselState->saveBox = pen::ui::AddItem(new pen::ui::Item(PEISEL_SAVE_BOX_ID,
            pen::Vec3(pen::PixelBufferWidth() / 10.0f * 8.0f, pen::PixelBufferHeight() - CANVAS_TOP_MARGIN, 0.0f), pen::Vec2(pen::PixelBufferWidth() / 20.0f - 10.0f, pen::PixelBufferWidth() / 20.0f - 10.0f),
            pen::ui::Shape::QUAD, pen::PEN_GREEN, nullptr, nullptr, true));

        pen::ui::Submit();
        pen::Pen::HandleGUIClickEvents(true, &Paint);
        pen::Pen::HandleGUIDragEvents(true);
        pen::Pen::HandleGUIKeyEvents(true);
    }

    void OnInput() override {
        switch (PeiselState::Get()->appMode) {
        case PEISEL_TILES:
            if (pen::Pen::KeyState(pen::in::KEYS::Q) == pen::in::KEYS::PRESSED && PeiselState::Get()->action == false) {
                /*Toggle forward to next tile sprite*/
                if (PeiselState::Get()->spriteSelect && PeiselState::Get()->spritesLoaded) {
                    if (PeiselState::Get()->selectedTileSlot == PeiselState::Get()->selectableTileSprites.size() - 1) {
                        PeiselState::Get()->selectedTileSlot = 0;
                    }
                    else {
                        PeiselState::Get()->selectedTileSlot++;
                    }
                    PeiselState::Get()->selectedTileSpriteName = PeiselState::Get()->selectableTileSprites[PeiselState::Get()->selectedTileSlot];
                    PeiselState::Get()->selectedTileSprite->UpdateText(ParseTileSpritePath(PeiselState::Get()->selectedTileSpriteName, 30));
                    PeiselState::Get()->action = true;
                    pen::ui::Submit();
                }
            }else if (pen::Pen::KeyState(pen::in::KEYS::TAB) == pen::in::KEYS::PRESSED && PeiselState::Get()->action == false) {
                /*Toggle backwards to previous tile sprite*/
                if (PeiselState::Get()->spriteSelect && PeiselState::Get()->spritesLoaded) {
                    if (PeiselState::Get()->selectedTileSlot == 0) {
                        PeiselState::Get()->selectedTileSlot = PeiselState::Get()->selectableTileSprites.size() - 1;
                    }
                    else {
                        PeiselState::Get()->selectedTileSlot--;
                    }
                    PeiselState::Get()->selectedTileSpriteName = PeiselState::Get()->selectableTileSprites[PeiselState::Get()->selectedTileSlot];
                    PeiselState::Get()->selectedTileSprite->UpdateText(ParseTileSpritePath(PeiselState::Get()->selectedTileSpriteName, 30));
                    PeiselState::Get()->action = true;
                    pen::ui::Submit();
                }
            }
            else if (pen::Pen::KeyState(pen::in::KEYS::Q) == pen::in::KEYS::RELEASED && pen::Pen::KeyState(pen::in::KEYS::TAB) == pen::in::KEYS::RELEASED) {
                PeiselState::Get()->action = false;
            }

            /*Toggle tile alt*/
            if (pen::Pen::KeyState(pen::in::KEYS::LEFT_SHIFT) == pen::in::KEYS::PRESSED && PeiselState::Get()->action == false) {
                PeiselState::Get()->action = true;
                PeiselState::Get()->changes = true;
                PeiselState::Get()->tileAlt = !PeiselState::Get()->tileAlt;
            }
            else if (pen::Pen::KeyState(pen::in::KEYS::LEFT_SHIFT) == pen::in::KEYS::RELEASED) {
                PeiselState::Get()->action = false;
            }
            break;
        default:
            break;
        }
    }

    void OnRender() override {
        while (pen::Pen::Running())
        {
            OnInput();
            if(pen::Render::Get()->firstTime) pen::Render::Background(pen::PEN_WHITE);

            HandleUpdates();

            pen::Pen::ManageRender();
        }
    }
};