/***************************************************************************
 * mgl_evalc.h is part of Math Graphic Library
 * Copyright (C) 2007 Alexey Balakin <balakin@appl.sci-nnov.ru>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//---------------------------------------------------------------------------
#ifndef _MGL_EVALC_H_
#define _MGL_EVALC_H_
//---------------------------------------------------------------------------
#include <complex>
/// short name of complex numbers
#define dual std::complex<double>
#include <mgl/mgl_eval.h>
//---------------------------------------------------------------------------
/// Class for evaluating formula specified by the string
class mglFormulaC					// ������ ��� ����� � ���������� ������
{
public:
	/// Evaluates the formula for 'x','r'=\a x, 'y','n'=\a y, 'z','t'=\a z, 'u'=\a u
	dual Calc(dual x,dual y=0,dual z=0,dual u=0);
	/// Evaluates the formula for 'x, y, z, u, v, w'
	dual Calc(dual x,dual y,dual z,dual u,dual v,dual w);
	/// Evaluates the formula for variables \a var
	dual Calc(const dual var[MGL_VS]);
	/// Return error code
	int GetError();
	/// Parse the formula \a str and create formula-tree
	mglFormulaC(const char *str);
	/// Clean up formula-tree
	~mglFormulaC();
protected:
	dual CalcIn(const dual *a1);
	mglFormulaC *Left,*Right;	// first and second argument of the function
	int Kod;					// the function ID
	dual Res;					// the number or the variable ID
	static int Error;
};
//---------------------------------------------------------------------------
#endif
