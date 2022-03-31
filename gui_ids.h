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

enum GUI_IDS {
	PEISEL_NAVBAR_ID = 0,
	PEISEL_CURRENT_COLOR_BOX_ID = 1,
	PEISEL_SAVE_BOX_ID = 2
};

enum TOOLS {
	PEISEL_BRUSH = 0,
	PEISEL_ERASER = 1,
	BRUSH_SMALL = 2,
	BRUSH_MEDIUM = 3,
	BRUSH_LARGE = 4,
	PEISEL_FILL = 5,
	PEISEL_COMBINER = 6
};

enum MODES {
	PEISEL_DRAW = 0,
	PEISEL_SHEET = 1,
	PEISEL_TILES = 2
};

enum DRAW_MODE_STATE {
	DRAW_BRUSHING = 0,
	DRAW_SHAPE = 1,
	DRAW_SHAPE_TRANSLATE = 3,
	DRAW_LINE = 4,
	DRAW_TRIANGLE = 5,
	DRAW_RECTANGLE = 6,
	DRAW_CIRCLE = 7
};

enum TILES {
	TILES_10 = 10,
	TILES_50 = 50,
	TILES_100 = 100,
	TILES_1000 = 1000
};