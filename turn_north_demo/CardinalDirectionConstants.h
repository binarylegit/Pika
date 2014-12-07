/*
 * Copyright (c) 2014 Daniel Fowler
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 * associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial 
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Array Index values for upper and lower x,y coordinates of the rejection region.
#define X_COORD_LOWER 0
#define Y_COORD_LOWER 1
#define X_COORD_UPPER 2
#define Y_COORD_UPPER 3

/***** NORTH *****/
// Right motor forward 50, Left motor reverse 50
extern const float RF50LR50_NORTH[4] = { 103.5, 672.7, 107.9, 675.9 };
// Right motor reverse 50, Left motor forward 50
extern const float RR50LF50_NORTH[4] = { 111.3, 665.0, 115.1, 668.7 };
// Right motor forward 100, Left motor forward 100
extern const float RF100LF100_NORTH[4] = { 125.9, 663.6, 132.0, 667.6 };
// Right motor reverse 100, Left motor reverse 100
extern const float RR100LR100_NORTH[4] = { 131.5, 660.9, 135.6, 665.4 };
// Right motor forward 0, Left motor forward 0
extern const float RF0LF0_NORTH[4] = { 103.2, 651.5, 106.6, 652.9};

/***** SOUTH *****/
// Right motor forward 50, Left motor reverse 50
extern const float RF50LR50_SOUTH[4] = { -188.1, -570.5, -184.1, -566.9 };
// Right motor reverse 50, Left motor forward 50
extern const float RR50LF50_SOUTH[4] = { -173.4, -574.3, -169.4, -570.4 };
// Right motor forward 100, Left motor forward 100
extern const float RF100LF100_SOUTH[4] = { -163.5, -576.5, -157.4, -572.4 };
// Right motor reverse 100, Left motor reverse 100
extern const float RR100LR100_SOUTH[4] = { -155.3, -575.2, -150.7, -570.9 };
// Right motor forward 0, Left motor forward 0
extern const float RF0LF0_SOUTH[4] = { -177.5, -573.9, -174.2, -572.7 };

/***** EAST *****/
// Right motor forward 50, Left motor reverse 50
extern const float RF50LR50_EAST[4] = { 17.7, -348.9, 21.3, -345.6 };
// Right motor reverse 50, Left motor forward 50
extern const float RR50LF50_EAST[4] = { 21.1, -350.4, 24.8, -346.7 };
// Right motor forward 100, Left motor forward 100
extern const float RF100LF100_EAST[4] = { 17.7, -351.4, 23.4, -347.5 };
// Right motor reverse 100, Left motor reverse 100
extern const float RR100LR100_EAST[4] = { 19.0, -351.4, 23.1, -347.3 };
// Right motor forward 0, Left motor forward 0
extern const float RF0LF0_EAST[4] = { 3.6, -352.4, 6.7, -350.9 };

/***** WEST *****/
// Right motor forward 50, Left motor reverse 50
extern const float RF50LR50_WEST[4] = { -17.6, -758.6, -13.8, -755.5 };
// Right motor reverse 50, Left motor forward 50
extern const float RR50LF50_WEST[4] = { -10.2, -762.0, -6.7, -758.3 };
// Right motor forward 100, Left motor forward 100
extern const float RF100LF100_WEST[4] = { -8.3, -759.5, -2.1, -755.5 };
// Right motor reverse 100, Left motor reverse 100
extern const float RR100LR100_WEST[4] = { -2.4, -757.6, 1.7, -753.1 };
// Right motor forward 0, Left motor forward 0
extern const float RF0LF0_WEST[4] = { -26.6, -777.5, -23.5, -775.9 };
