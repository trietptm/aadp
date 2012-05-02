// $Id$

/*
 Anti-Anti-Debugging Plugin for Ollydbg v0.3 - LGPL 3.0

 Copyright (C) 2012 +NCR/CRC! [ReVeRsEr] http://crackinglandia.blogspot.com

 This library is free software: you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __AADPCONSTANTS_H__
#define __AADPCONSTANTS_H__

#define MAX_BYTES 4096
#define USEDEFAULT 0x80000000

#define TABAADBTRICKS		11
#define TABOLLYFIXES		12
#define TABADVSETTINGS		13
#define TABCUSTOMSETTINGS	14

#define SIZEAADBTRICKSARRAY 18
#define SIZEOLLYFIXESARRAY	5
#define SIZEADVSETTINGSARRAY 12

#define NOCOUNTER 1
#define COUNTERPLUSONE 2
#define RANDOMCOUNTER 3

#define RTN_OK 1
#define RTN_USAGE 0
#define RTN_ERROR 13

#define VERSIONHI		1
#define VERSIONLO		10
#define HIGHLIGHTED		1
#define GRAYED			-1
#define STANDARD		0
#endif