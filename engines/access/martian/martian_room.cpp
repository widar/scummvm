/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "access/access.h"
#include "access/resources.h"
#include "access/martian/martian_game.h"
#include "access/martian/martian_resources.h"
#include "access/martian/martian_room.h"

namespace Access {

namespace Martian {

MartianRoom::MartianRoom(AccessEngine *vm) : Room(vm) {
	_game = (MartianEngine *)vm;
}

MartianRoom::~MartianRoom() {
}

void MartianRoom::loadRoom(int roomNumber) {
	loadRoomData(ROOM_TABLE[roomNumber]);
}

void MartianRoom::reloadRoom() {
	loadRoom(_vm->_player->_roomNumber);

	if (_roomFlag != 1) {
		_vm->_currentMan = _roomFlag;
		_vm->_currentManOld = _roomFlag;
		_vm->_manScaleOff = 0;

		switch (_vm->_currentMan) {
		case 0:
			_vm->_player->loadSprites("MAN.LZ");
			break;

		case 2:
			_vm->_player->loadSprites("JMAN.LZ");
			break;

		case 3:
			_vm->_player->loadSprites("OVERHEAD.LZ");
			_vm->_manScaleOff = 1;
			break;

		default:
			break;
		}
	}

	reloadRoom1();
}

void MartianRoom::reloadRoom1() {
	if (_vm->_player->_roomNumber == 29 || _vm->_player->_roomNumber == 31
			|| _vm->_player->_roomNumber == 42 || _vm->_player->_roomNumber == 44) {
		//Resource *spriteData = _vm->_files->loadFile("MAYA.LZ");
		//_vm->_inactive._spritesPtr = new SpriteResource(_vm, spriteData);
		//delete spriteData;
		_vm->_currentCharFlag = false;
	}

	_selectCommand = -1;
	_vm->_events->setNormalCursor(CURSOR_CROSSHAIRS);
	_vm->_mouseMode = 0;
	_vm->_boxSelect = true;
	_vm->_player->_playerOff = false;

	_vm->_screen->fadeOut();
	_vm->_screen->clearScreen();
	roomSet();

	// TODO: Refactor

	_vm->_screen->setBufferScan();
	setupRoom();
	setWallCodes();
	buildScreen();

	if (!_vm->_screen->_vesaMode) {
		_vm->copyBF2Vid();
	} else if (_vm->_player->_roomNumber != 20 && _vm->_player->_roomNumber != 24
			&& _vm->_player->_roomNumber != 33) {
		_vm->_screen->setPalette();
		_vm->copyBF2Vid();
	}

	_vm->_player->_frame = 0;
	_vm->_oldRects.clear();
	_vm->_newRects.clear();
}

void MartianRoom::roomSet() {
	_vm->_numAnimTimers = 0;
	_vm->_scripts->_sequence = 1000;
	_vm->_scripts->searchForSequence();
	_vm->_scripts->executeScript();
}

void MartianRoom::roomMenu() {
	Resource *iconData = _vm->_files->loadFile("ICONS.LZ");
	SpriteResource *spr = new SpriteResource(_vm, iconData);
	delete iconData;

	_vm->_screen->saveScreen();
	_vm->_screen->setDisplayScan();
	_vm->_destIn = _vm->_screen;	// TODO: Redundant
	_vm->_screen->plotImage(spr, 0, Common::Point(0, 177));
	_vm->_screen->plotImage(spr, 1, Common::Point(143, 177));

	_vm->_screen->restoreScreen();
	delete spr;
}

void MartianRoom::mainAreaClick() {
}

} // End of namespace Martian

} // End of namespace Access
