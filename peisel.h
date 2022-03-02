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

PeiselState* PeiselState::instance = nullptr;

class App : public pen::Pen {
public:
    

public:
    void CheckSave() {
        if(PeiselState::Get()->changes) {
            if (PeiselState::Get()->saveBox->color.x != pen::PEN_RED.x) {
                PeiselState::Get()->saveBox->SetColor(pen::PEN_RED);
                pen::ui::Submit();
            }
        }
    }

    void HandleUpdates() {
        CheckSave();
        switch (PeiselState::Get()->appMode) {
        case PEISEL_DRAW:
            DrawMode();
            UpdateCanvas();
            break;
        case PEISEL_SHEET:
            OnSpriteSheetMode();
            break;
        default:
            break;
        }
    }

    void OnCreate() override {
        pen::InitializePixels();
        pen::ui::Item* pixelBuffer = pen::ui::LM::pixelLayer->layerItems[0];
        pixelBuffer->size.y = pixelBuffer->size.y - CANVAS_TOP_MARGIN;

        /*Allocates a buffer for free drawing*/
        PeiselState* peiselState = PeiselState::Get();
        peiselState->freeDrawBuffer = new unsigned char[pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4];
        std::memset(peiselState->freeDrawBuffer, 0, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);

        optionList = { "", nullptr };
        peiselState->navBar = pen::ui::AddItem(new pen::ui::NavBar(PEISEL_NAVBAR_ID, CANVAS_TOP_MARGIN, pen::PEN_LIGHT_GRAY));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "File", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Tools", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 5, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Draw", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Erase", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 5, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Size", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));
        peiselState->navBar->Push(new pen::ui::Button(ID_ANY, "Mode", pen::Vec3(0.0f, pen::Pen::ScreenHeight() - 40.0f, 0.0f), 4, pen::PEN_LIGHT_GRAY, pen::PEN_BLACK, peiselState->navBar, &NavClick, true));

        peiselState->currentColorBox = pen::ui::AddItem(new pen::ui::Item(PEISEL_CURRENT_COLOR_BOX_ID, 
            pen::Vec3(pen::Pen::ScreenWidth() / 10.0f * 9.0f, pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN, 0.0f), pen::Vec2(pen::Pen::ScreenWidth() / 10.0f - 10.0f, CANVAS_TOP_MARGIN),
            pen::ui::Shape::QUAD, pen::PEN_LIGHT_GRAY, nullptr, nullptr, true));
        peiselState->currentColorBox->Push(new pen::ui::Item(ID_ANY, 
            pen::Vec3(peiselState->currentColorBox->positions.x + 4.0f, peiselState->currentColorBox->positions.y + 4.0f, 0.0f),
            pen::Vec2(peiselState->currentColorBox->size.x - 8.0f, peiselState->currentColorBox->size.y - 8.0f),
            pen::ui::Shape::QUAD, pen::PEN_BLACK, peiselState->currentColorBox, nullptr, true));

        peiselState->saveBox = pen::ui::AddItem(new pen::ui::Item(PEISEL_SAVE_BOX_ID,
            pen::Vec3(pen::Pen::ScreenWidth() / 10.0f * 8.0f, pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN, 0.0f), pen::Vec2(pen::Pen::ScreenWidth() / 20.0f - 10.0f, pen::Pen::ScreenWidth() / 20.0f - 10.0f),
            pen::ui::Shape::QUAD, pen::PEN_GREEN, nullptr, nullptr, true));

        pen::ui::Submit();
        pen::Pen::HandleGUIClickEvents(true, &Paint);
        pen::Pen::HandleGUIDragEvents(true);
        pen::Pen::HandleGUIKeyEvents(true);
    }

    void OnInput() override {
        
    }

    void OnRender() override {
        pen::Renderer renderer;

        while (pen::Pen::Running())
        {
            pen::Pen::OnInput();
            renderer.Clear();
            if(pen::Render::Get()->firstTime) renderer.Background(pen::PEN_WHITE);

            HandleUpdates();

            pen::Pen::ManageRender(&renderer);
        }
    }
};