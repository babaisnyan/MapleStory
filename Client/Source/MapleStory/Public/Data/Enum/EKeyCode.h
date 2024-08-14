﻿#pragma once

UENUM(BlueprintType)
enum class EKeyCode : uint8 {
	None UMETA(DisplayName = "없음"),
	Esc UMETA(DisplayName = "ESC"),
	One UMETA(DisplayName = "1"),
	Two UMETA(DisplayName = "2"),
	Three UMETA(DisplayName = "3"),
	Four UMETA(DisplayName = "4"),
	Five UMETA(DisplayName = "5"),
	Six UMETA(DisplayName = "6"),
	Seven UMETA(DisplayName = "7"),
	Eight UMETA(DisplayName = "8"),
	Nine UMETA(DisplayName = "9"),
	Zero UMETA(DisplayName = "0"),
	Minus UMETA(DisplayName = "-"),
	Equal UMETA(DisplayName = "="),
	Q = 16 UMETA(DisplayName = "Q"),
	W UMETA(DisplayName = "W"),
	E UMETA(DisplayName = "E"),
	R UMETA(DisplayName = "R"),
	T UMETA(DisplayName = "T"),
	Y UMETA(DisplayName = "Y"),
	U UMETA(DisplayName = "U"),
	I UMETA(DisplayName = "I"),
	O UMETA(DisplayName = "O"),
	P UMETA(DisplayName = "P"),
	OpenBracket UMETA(DisplayName = "["),
	CloseBracket UMETA(DisplayName = "]"),
	Ctrl = 29 UMETA(DisplayName = "Ctrl"),
	A UMETA(DisplayName = "A"),
	S UMETA(DisplayName = "S"),
	D UMETA(DisplayName = "D"),
	F UMETA(DisplayName = "F"),
	G UMETA(DisplayName = "G"),
	H UMETA(DisplayName = "H"),
	J UMETA(DisplayName = "J"),
	K UMETA(DisplayName = "K"),
	L UMETA(DisplayName = "L"),
	Semicolon UMETA(DisplayName = ";"),
	Quote UMETA(DisplayName = "'"),
	Tilde UMETA(DisplayName = "`"),
	Shift UMETA(DisplayName = "Shift"),
	BackSlash UMETA(DisplayName = "\\"),
	Z UMETA(DisplayName = "Z"),
	X UMETA(DisplayName = "X"),
	C UMETA(DisplayName = "C"),
	V UMETA(DisplayName = "V"),
	B UMETA(DisplayName = "B"),
	N UMETA(DisplayName = "N"),
	M UMETA(DisplayName = "M"),
	Comma UMETA(DisplayName = ","),
	Period UMETA(DisplayName = "."),
	Alt = 56 UMETA(DisplayName = "Alt"),
	Space UMETA(DisplayName = "Space"),
	F1 = 59 UMETA(DisplayName = "F1"),
	F2 UMETA(DisplayName = "F2"),
	F3 UMETA(DisplayName = "F3"),
	F4 UMETA(DisplayName = "F4"),
	F5 UMETA(DisplayName = "F5"),
	F6 UMETA(DisplayName = "F6"),
	F7 UMETA(DisplayName = "F7"),
	F8 UMETA(DisplayName = "F8"),
	F9 UMETA(DisplayName = "F9"),
	F10 UMETA(DisplayName = "F10"),
	ScrollLock = 70 UMETA(DisplayName = "ScrollLock"),
	Home UMETA(DisplayName = "Home"),
	PageUp = 73 UMETA(DisplayName = "PageUp"),
	End = 79 UMETA(DisplayName = "End"),
	PageDown = 81 UMETA(DisplayName = "PageDown"),
	Insert UMETA(DisplayName = "Insert"),
	Delete UMETA(DisplayName = "Delete"),
	F11 = 87 UMETA(DisplayName = "F11"),
	F12 UMETA(DisplayName = "F12")
};
