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

void PeiselFill(int x, int y) {
    /*Fills an area with the selected color*/
    PeiselState* peiselState = PeiselState::Get();
    pen::Vec4 origColor = pen::Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    unsigned char* pixels = pen::PixelBuffer();
    origColor.x = (float)(pixels[y * (pen::PixelBufferWidth() * 4) + (x * 4)] / 255.0f);
    origColor.y = (float)(pixels[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 1] / 255.0f);
    origColor.z = (float)(pixels[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 2] / 255.0f);
    origColor.w = (float)(pixels[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 3] / 255.0f);

    bool topFinished = false;
    bool bottomFinished = false;
    bool leftFinished = false;
    bool rightFinished = false;
    
    int heightCounter = y;
    int widthCounter = x;

    /*Check if fill is not needed*/
    if (origColor.x == peiselState->currentColor.x
        && origColor.y == peiselState->currentColor.y
        && origColor.z == peiselState->currentColor.z
        && origColor.w == peiselState->currentColor.w) {
        topFinished = true;
        bottomFinished = true;
    }

    while (!(topFinished && bottomFinished)) {
        /*Go upwards then downwards to fill within region until boundary of different color from origColor is hit*/
        while (!(leftFinished && rightFinished)) {
            /*Once left and right boundaries are hit go up or down*/
            if (!leftFinished) {
                if (origColor.x != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4)] / 255.0f)
                    || origColor.y != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 1] / 255.0f)
                    || origColor.z != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 2] / 255.0f)
                    || origColor.w != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 3] / 255.0f)) {
                    leftFinished = true;
                    widthCounter = x + 2;
                }
                else {
                    pen::Draw(widthCounter, heightCounter, peiselState->currentColor);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4)] = (unsigned char)(255 * peiselState->currentColor.x);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 1] = (unsigned char)(255 * peiselState->currentColor.y);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 2] = (unsigned char)(255 * peiselState->currentColor.z);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 3] = (unsigned char)(255 * peiselState->currentColor.w);
                }

                widthCounter--;
                if (widthCounter < 0) {
                    leftFinished = true;
                    widthCounter = x + 1;
                }
            }
            else {
                if (origColor.x != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4)] / 255.0f)
                    || origColor.y != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 1] / 255.0f)
                    || origColor.z != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 2] / 255.0f)
                    || origColor.w != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 3] / 255.0f)) {
                    rightFinished = true;
                    widthCounter = x - 1;
                }
                else {
                    pen::Draw(widthCounter, heightCounter, peiselState->currentColor);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4)] = (unsigned char)(255 * peiselState->currentColor.x);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 1] = (unsigned char)(255 * peiselState->currentColor.y);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 2] = (unsigned char)(255 * peiselState->currentColor.z);
                    peiselState->freeDrawBuffer[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 3] = (unsigned char)(255 * peiselState->currentColor.w);
                }

                widthCounter++;
                if (widthCounter > pen::PixelBufferWidth() - 1) {
                    rightFinished = true;
                    widthCounter = x;
                }
            }
        }

        heightCounter += (!topFinished ? 1 : -1);
        leftFinished = false;
        rightFinished = false;
        if (heightCounter > pen::PixelBufferHeight() - 1) {
            topFinished = true;
            heightCounter = y - 1;
        }
        else if (heightCounter < 0) {
            bottomFinished = true;
        }else if (origColor.x != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4)] / 255.0f)
            || origColor.y != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 1] / 255.0f)
            || origColor.z != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 2] / 255.0f)
            || origColor.w != (float)(pixels[heightCounter * (pen::PixelBufferWidth() * 4) + (widthCounter * 4) + 3] / 255.0f)) {
            if (!topFinished) {
                topFinished = true;
            }
            else {
                bottomFinished = true;
            }
            heightCounter = y - 1;
        }
    }   

    peiselState->action = true;
    peiselState->changes = true;
}

void PaintCircle(int x, int y, int radius, pen::Vec4 color, bool fill = true) {
    /*Create a circle to be loaded in as a sprite*/
    PeiselState* peiselState = PeiselState::Get();
    for (float rad = 0.0f; rad < 2.0f * 3.14159f; rad += 0.01f) {
        int xOffset = x + radius * pen::op::Sin(rad);
        int yOffset = y + radius * pen::op::Cos(rad);

        pen::Draw(xOffset, yOffset, color);

        if ((peiselState->tool == PEISEL_BRUSH && peiselState->currentColor.w != 0.0f) || peiselState->tool == PEISEL_ERASER) {
            peiselState->freeDrawBuffer[yOffset * (pen::PixelBufferWidth() * 4) + (xOffset * 4)] = (unsigned char)(255 * color.x);
            peiselState->freeDrawBuffer[yOffset * (pen::PixelBufferWidth() * 4) + (xOffset * 4) + 1] = (unsigned char)(255 * color.y);
            peiselState->freeDrawBuffer[yOffset * (pen::PixelBufferWidth() * 4) + (xOffset * 4) + 2] = (unsigned char)(255 * color.z);
            peiselState->freeDrawBuffer[yOffset * (pen::PixelBufferWidth() * 4) + (xOffset * 4) + 3] = (unsigned char)(255 * color.w);
        }

        if (fill && rad <= 3.14259f) {
            /*Fill the circle via a vertical reflection*/
            int oppositeXOffset = x - radius * pen::op::Sin(3.14159f - rad);
            int oppositeYOffset = y + radius * pen::op::Cos(3.14159f - rad);
            int leftBound = xOffset < oppositeXOffset ? xOffset : oppositeXOffset;
            int rightBound = xOffset > oppositeXOffset ? xOffset : oppositeXOffset;
            int bottomBound = yOffset < oppositeYOffset ? yOffset : oppositeYOffset;
            int topBound = yOffset > oppositeYOffset ? yOffset : oppositeYOffset;

            for (int fillX = leftBound; fillX < rightBound; fillX++) {
                for (int fillY = bottomBound; fillY < topBound; fillY++) {
                    pen::Draw(fillX, fillY, color);
                    if ((peiselState->tool == PEISEL_BRUSH && peiselState->currentColor.w != 0.0f) || peiselState->tool == PEISEL_ERASER) {
                        peiselState->freeDrawBuffer[fillY * (pen::PixelBufferWidth() * 4) + (fillX * 4)] = (unsigned char)(255 * color.x);
                        peiselState->freeDrawBuffer[fillY * (pen::PixelBufferWidth() * 4) + (fillX * 4) + 1] = (unsigned char)(255 * color.y);
                        peiselState->freeDrawBuffer[fillY * (pen::PixelBufferWidth() * 4) + (fillX * 4) + 2] = (unsigned char)(255 * color.z);
                        peiselState->freeDrawBuffer[fillY * (pen::PixelBufferWidth() * 4) + (fillX * 4) + 3] = (unsigned char)(255 * color.w);
                    }
                }
            }
        }
    }
}

void PeiselUpdateLine(pen::Item* data, int startX, int startY, int endX, int endY, pen::Vec4 color) {
    /*Create a line segment to be drawn*/

    int startBoundX = 0;
    int startBoundY = 0;
    int width = pen::op::Sqrt(pen::op::Pow(endX - startX, 2) + pen::op::Pow(endY - startY, 2)) + 1;
    int height = endY - startY + 1;
    if (width > 5 && height > 5) {
        data->width = width;
        data->height = height;
        if (data->data != nullptr) {
            delete[] data->data;
        }
        unsigned char* newData = new unsigned char[width * height * 4];
        std::memset(newData, 0, width * height * 4);
        float changeY = endY - startY;
        float changeX = endX - startX;
        if (changeX == 0.0f) changeX = 0.0001f;
        float slope = changeY / changeX;

        if (startX > endX) {
            int tempX = startX;
            int tempY = startY;
            startX = endX;
            startY = endY;
            endX = tempX;
            endY = tempY;
        }

        for (int x = startBoundX; x < width; x++) {
            int y = (slope * x) - (slope * startBoundX) + (startBoundY);
            pen::UpdateSprite(newData, x, y, width, height, color);

            /*Slope thresholds to make the line look more connected*/
            if (pen::op::Abs(slope) > 4.4f) {
                pen::UpdateSprite(newData, x, y + 1, width, height, color);
                pen::UpdateSprite(newData, x, y - 1, width, height, color);
                pen::UpdateSprite(newData, x, y + 2, width, height, color);
                pen::UpdateSprite(newData, x, y + 3, width, height, color);
            }
            else if (pen::op::Abs(slope) > 3.5f) {
                pen::UpdateSprite(newData, x, y + 1, width, height, color);
                pen::UpdateSprite(newData, x, y - 1, width, height, color);
                pen::UpdateSprite(newData, x, y + 2, width, height, color);
            }
            else if (pen::op::Abs(slope) > 2.4f) {
                pen::UpdateSprite(newData, x, y + 1, width, height, color);
                pen::UpdateSprite(newData, x, y - 1, width, height, color);
            }
            else if (pen::op::Abs(slope) > 1.1f) {
                pen::UpdateSprite(newData, x, y + 1, width, height, color);
                pen::UpdateSprite(newData, x, y + 2, width, height, color);
            }
        }

        data->data = newData;
    }
}

void UpdateCanvas() {
    /*Redraws all of the buffer data*/
    PeiselState* peiselState = PeiselState::Get();
    if (peiselState->updateCanvas) {
        pen::Flush();
        std::memcpy(pen::PixelBuffer(), peiselState->freeDrawBuffer, pen::PixelBufferWidth() * pen::PixelBufferHeight() * 4);
        for (int i = 0; i < peiselState->shapes.size(); i++) {
            peiselState->shapes[i]->item->Draw();
        }
        peiselState->updateCanvas = false;
    }
}

void UpdateEditBox(bool finished) {
    /*Update the bounding box of the current shape*/
    PeiselState* peiselState = PeiselState::Get();
    if (peiselState->currentShapeData != nullptr && peiselState->editBox == nullptr) {
        peiselState->editBox = pen::DrawRect(peiselState->currentShapeData->item->x - 4, peiselState->currentShapeData->item->y - 4,
            peiselState->currentShapeData->item->width + 8, peiselState->currentShapeData->item->height + 8, pen::PEN_CYAN, true);
    }

    if (finished && peiselState->editBox != nullptr) {
        pen::DeleteItem(peiselState->editBox);
        peiselState->editBox = nullptr;
    }
}

PeiselShape* DrawHandleClick(PeiselState* peiselState, int& x, int& y) {
    pen::Pen::GetMousePos(peiselState->mouseXPtr, peiselState->mouseYPtr);

    if (*peiselState->mouseXPtr >= 0.0f && *peiselState->mouseXPtr <= pen::Pen::ScreenWidth() && *peiselState->mouseYPtr >= 0.0f && *peiselState->mouseYPtr <= pen::Pen::ScreenHeight() - CANVAS_TOP_MARGIN) {
        x = PeiselState::ScreenToPexelX(*peiselState->mouseXPtr);
        y = PeiselState::ScreenToPexelY(*peiselState->mouseYPtr);

        if (PeiselState::Get()->appState != DRAW_SHAPE) {
            if ((pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::HELD)
                || (peiselState->appState == DRAW_SHAPE_TRANSLATE)) {
                const unsigned int listSize = peiselState->shapes.size();
                for (int i = 0; i < listSize; i++) {
                    if (peiselState->shapes[listSize - i - 1]->type == DRAW_CIRCLE) {
                        /*Shifts the hit box down and left since the x and y for a circle is in the center instead of the bottom left*/
                        if (peiselState->shapes[listSize - i - 1]->item->x - peiselState->shapes[listSize - i - 1]->item->width / 2 < x 
                            && peiselState->shapes[listSize - i - 1]->item->x + peiselState->shapes[listSize - i - 1]->item->width / 2 * peiselState->shapes[listSize - i - 1]->scaleX > x
                            && peiselState->shapes[listSize - i - 1]->item->y - peiselState->shapes[listSize - i - 1]->item->height / 2 < y 
                            && peiselState->shapes[listSize - i - 1]->item->y + peiselState->shapes[listSize - i - 1]->item->height / 2 * peiselState->shapes[listSize - i - 1]->scaleY > y) {
                            return peiselState->shapes[listSize - i - 1];
                        }
                    }
                    else {
                        if (peiselState->shapes[listSize - i - 1]->item->x < x 
                            && peiselState->shapes[listSize - i - 1]->item->x + peiselState->shapes[listSize - i - 1]->item->width * peiselState->shapes[listSize - i - 1]->scaleX > x
                            && peiselState->shapes[listSize - i - 1]->item->y < y 
                            && peiselState->shapes[listSize - i - 1]->item->y + peiselState->shapes[listSize - i - 1]->item->height * peiselState->shapes[listSize - i - 1]->scaleY > y) {
                            return peiselState->shapes[listSize - i - 1];
                        }
                    }
                }
            }
        }
    }
    else {
        x = -1;
        y = -1;
    }

    return nullptr;
}

bool Paint() {
    /*Check for click within the bounds of the drawing container*/
    int x = 0;
    int y = 0;
    PeiselState* peiselState = PeiselState::Get();
    PeiselShape* shapeToEdit = DrawHandleClick(peiselState, x, y);

    if (shapeToEdit != nullptr) peiselState->appState = DRAW_SHAPE_TRANSLATE;

    if (x > 0) {
        if (optionList.data != nullptr && peiselState->action) {
            pen::ui::RemoveItem(optionList.data);
            pen::ui::Submit();
            optionList = { "", nullptr };
        }

        if (peiselState->tool == PEISEL_FILL && pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED) {
            PeiselFill(x, y);
            return true;
        }

        switch (peiselState->appState) {
        case DRAW_BRUSHING:
            /*Get brush drawing started*/
            if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED || pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::HELD) {
                /*Close any option lists that are open*/
                peiselState->changes = true;
                peiselState->currentShape = 0;

                /*Convert screen coordinates to texel coordinates*/
                peiselState->penActive = true;

                switch (peiselState->brushSize) {
                case BRUSH_SMALL:
                    pen::Draw(x, y, peiselState->currentColor, (peiselState->tool == PEISEL_BRUSH ? true : false));
                    if ((peiselState->tool == PEISEL_BRUSH && peiselState->currentColor.w != 0.0f) || peiselState->tool == PEISEL_ERASER) {
                        peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4)] = (unsigned char)(255 * peiselState->currentColor.x);
                        peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 1] = (unsigned char)(255 * peiselState->currentColor.y);
                        peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 2] = (unsigned char)(255 * peiselState->currentColor.z);
                        peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 3] = (unsigned char)(255 * peiselState->currentColor.w);
                    }
                    break;
                case BRUSH_MEDIUM:
                    PaintCircle(x, y, 15, peiselState->currentColor);
                    break;
                case BRUSH_LARGE:
                    PaintCircle(x, y, 30, peiselState->currentColor);
                    break;
                default: 
                    break;
                }
                
                peiselState->action = true;
            }
            else {
                /*User has released the pen*/
                peiselState->changes = true;
                peiselState->action = false;
                peiselState->penActive = false;
            }
            break;
        case DRAW_SHAPE:
            /*Draw an initial shape based on user's selection with them still being able to resize it until they release their click*/
            if (peiselState->currentShape != 0) {
                if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::PRESSED) {
                    pen::Item* item = nullptr;
                    peiselState->changes = true;
                    peiselState->penActive = true;
                    peiselState->action = true;

                    if (peiselState->currentShapeData != nullptr) {
                        /*Resize the shape*/
                        if (peiselState->currentShapeData->type == DRAW_LINE) {
                            /*Redraw the line buffer based on the cursor*/
                            int startX = peiselState->currentShapeData->item->x;
                            int startY = peiselState->currentShapeData->item->y;
                            PeiselUpdateLine(peiselState->currentShapeData->item, startX, startY, x, y, peiselState->currentColor);
                        }
                        else {
                            if (x < pen::PixelBufferWidth() / 2) {
                                /*Shrink the shape*/
                                pen::Scale(peiselState->currentShapeData->item, 0.995f, 0.995f);
                                peiselState->currentShapeData->scaleX *= 0.995f;
                                peiselState->currentShapeData->scaleY *= 0.995f;
                                if(peiselState->editBox != nullptr) pen::Scale(peiselState->editBox, 0.995f, 0.995f);
                            }
                            else {
                                /*Enlarge the shape*/
                                pen::Scale(peiselState->currentShapeData->item, 1.005f, 1.005f);
                                peiselState->currentShapeData->scaleX *= 1.005f;
                                peiselState->currentShapeData->scaleY *= 1.005f;
                                if (peiselState->editBox != nullptr)pen::Scale(peiselState->editBox, 1.005f, 1.005f);
                            }
                        }
                    }
                    else {
                        /*Ten percent in both directions for initial shapes*/
                        int width = 100;
                        int height = 50;

                        if (x < 100) width = x;
                        if (x > pen::PixelBufferWidth()) x -= 200;
                        if (y < 50) height = y;
                        if (y > pen::PixelBufferHeight() - CANVAS_TOP_MARGIN) y -= 100;

                        switch (peiselState->currentShape) {
                        case DRAW_LINE:
                            item = pen::DrawLine(x - width, y - height, x, y, peiselState->currentColor);
                            break;
                        case DRAW_TRIANGLE:
                            item = pen::DrawTri(x - width, y - height, x, y - height, x - (width / 2), y, peiselState->currentColor, false);
                            break;
                        case DRAW_RECTANGLE:
                            item = pen::DrawRect(x - width, y - height, width, height, peiselState->currentColor, false);
                            break;
                        case DRAW_CIRCLE:
                            item = pen::DrawCircle(x, y, height, peiselState->currentColor, false);
                            break;
                        default:
                            break;
                        }

                        if (item != nullptr) {
                            peiselState->currentShapeData = new PeiselShape{ peiselState->currentShape, item };
                            peiselState->shapes.push_back(peiselState->currentShapeData);
                            peiselState->updateCanvas = true;
                            UpdateEditBox(false);
                        }
                    }
                }
                else if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::RELEASED) {
                    /*User has released their click so now the shape is finished*/  
                    if (peiselState->currentShapeData != nullptr) {
                        /*This check has to be made since this can happen before user actually draws a shape*/
                        UpdateEditBox(true);
                        peiselState->changes = true;
                        peiselState->currentShapeData = nullptr;
                        peiselState->updateCanvas = true;
                        peiselState->currentShape = 0;
                        peiselState->appState = DRAW_BRUSHING;
                        peiselState->penActive = false;
                        peiselState->action = false;
                        PeiselState::Get()->currentColorBox->SetColor(pen::PEN_LIGHT_GRAY);
                        pen::ui::Submit();
                    }
                }
            }
            break;
        case DRAW_SHAPE_TRANSLATE:
            /*User has decided to move the shape*/
            if (shapeToEdit != nullptr) {
                if (pen::Pen::MouseState(pen::in::KEYS::MOUSE_LEFT) == pen::in::KEYS::RELEASED) {
                    UpdateEditBox(true);
                    peiselState->changes = true;
                    peiselState->penActive = false;
                    peiselState->action = false;
                    peiselState->currentShapeData = nullptr;
                    peiselState->appState = DRAW_BRUSHING;
                    peiselState->updateCanvas = true;
                }
                else {
                    peiselState->changes = true;
                    peiselState->penActive = true;
                    peiselState->action = true;
                    peiselState->currentShapeData = shapeToEdit;
                    int newX = x - peiselState->currentShapeData->item->width / 2;
                    int newY = y - peiselState->currentShapeData->item->height / 2;

                    peiselState->currentShapeData->item->x = newX;
                    peiselState->currentShapeData->item->y = newY;
                    peiselState->updateCanvas = true;
                    UpdateEditBox(false);
                }
            }
            break;
        default:
            break;
        }
    }
    return true;
}

void DrawMode() {
    if (PeiselState::Get()->penActive) {
        PeiselState* peiselState = PeiselState::Get();
        pen::Pen::GetMousePos(peiselState->mouseXPtr, peiselState->mouseYPtr);
        int x = PeiselState::ScreenToPexelX(*peiselState->mouseXPtr);
        int y = PeiselState::ScreenToPexelY(*peiselState->mouseYPtr);
        switch (peiselState->appState) {
        case DRAW_BRUSHING:
            /*Continue brush drawing since user's click is still being held*/
            /*User is drawing*/
            switch (peiselState->brushSize) {
            case BRUSH_SMALL:
                pen::Draw(x, y, peiselState->currentColor, (peiselState->tool == PEISEL_BRUSH ? true : false));
                if ((peiselState->tool == PEISEL_BRUSH && peiselState->currentColor.w != 0.0f) || peiselState->tool == PEISEL_ERASER) {
                    peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4)] = (unsigned char)(255 * peiselState->currentColor.x);
                    peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 1] = (unsigned char)(255 * peiselState->currentColor.y);
                    peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 2] = (unsigned char)(255 * peiselState->currentColor.z);
                    peiselState->freeDrawBuffer[y * (pen::PixelBufferWidth() * 4) + (x * 4) + 3] = (unsigned char)(255 * peiselState->currentColor.w);
                }
                break;
            case BRUSH_MEDIUM:
                PaintCircle(x, y, 15, peiselState->currentColor);
                break;
            case BRUSH_LARGE:
                PaintCircle(x, y, 30, peiselState->currentColor);
                break;
            default:
                break;
            }
            break;
        case DRAW_SHAPE:
            /*Resize current shape until user releases their click*/
            if (peiselState->currentShape != 0 && peiselState->currentShapeData != nullptr) {
                /*Resize the shape*/
                if (peiselState->currentShapeData->type == DRAW_LINE) {
                    /*Redraw the line buffer based on the cursor*/
                    int startX = peiselState->currentShapeData->item->x;
                    int startY = peiselState->currentShapeData->item->y;
                    PeiselUpdateLine(peiselState->currentShapeData->item, startX, startY, x, y, peiselState->currentColor);
                }
                else {
                    if (x < pen::PixelBufferWidth() / 2) {
                        /*Shrink the shape*/
                        pen::Scale(peiselState->currentShapeData->item, 0.995f, 0.995f);
                        peiselState->currentShapeData->scaleX *= 0.995f;
                        peiselState->currentShapeData->scaleY *= 0.995f;
                        if (peiselState->editBox != nullptr) pen::Scale(peiselState->editBox, 0.995f, 0.995f);
                    }
                    else {
                        /*Enlarge the shape*/
                        pen::Scale(peiselState->currentShapeData->item, 1.005f, 1.005f);
                        peiselState->currentShapeData->scaleX *= 1.005f;
                        peiselState->currentShapeData->scaleY *= 1.005f;
                        if (peiselState->editBox != nullptr)pen::Scale(peiselState->editBox, 1.005f, 1.005f);
                    }
                }
                peiselState->updateCanvas = true;
            }
            break;
        case DRAW_SHAPE_TRANSLATE:
            /*Continue translating the shape*/
            if (peiselState->currentShapeData != nullptr) {
                int newX = x - peiselState->currentShapeData->item->width / 2;
                int newY = y - peiselState->currentShapeData->item->height / 2;

                peiselState->currentShapeData->item->x = newX;
                peiselState->currentShapeData->item->y = newY;
                if (peiselState->editBox != nullptr) {
                    peiselState->editBox->x = newX;
                    peiselState->editBox->y = newY;
                }
                peiselState->updateCanvas = true;
            }
            break;
        default:
            break;
        }  
    }
}