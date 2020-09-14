// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 1) throw exception("Illegal argument.");
	BitLen = len;
	MemLen = (BitLen - 1) / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0u;
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
	this->pMem = new TELEM[this->MemLen];
	for (int i = 0; i < this->MemLen; i++)
		this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n > BitLen) throw exception("Negative or too large index.");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen) throw exception("Negative or too large index.");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen) throw exception("Negative or too large index.");
	return (pMem[GetMemIndex(n)] >> (n % (sizeof(TELEM) * 8))) & 1u;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete[] this->pMem;
	this->BitLen = bf.BitLen;
	this->MemLen = bf.MemLen;
	this->pMem = new TELEM[this->MemLen];
	for (int i = 0; i < this->MemLen; i++)
		this->pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->BitLen != bf.BitLen) return 0;
	else
		for (int i = 0; i < this->BitLen; i++)
			if (this->GetBit(i) != bf.GetBit(i)) return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField temp(this->BitLen > bf.BitLen ? this->BitLen : bf.BitLen);
	if (this->BitLen > bf.BitLen)
	{
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] = bf.pMem[i];
		for (int i = 0; i < this->MemLen; i++)
			temp.pMem[i] |= this->pMem[i];
	}
	else
	{
		for (int i = 0; i < this->MemLen; i++)
			temp.pMem[i] = this->pMem[i];
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] |= bf.pMem[i];
	}
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField temp(this->BitLen > bf.BitLen ? this->BitLen : bf.BitLen);
	if (this->BitLen > bf.BitLen)
	{
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] = bf.pMem[i];
		for (int i = 0; i < this->MemLen; i++)
			temp.pMem[i] &= this->pMem[i];
	}
	else
	{
		for (int i = 0; i < this->MemLen; i++)
			temp.pMem[i] = this->pMem[i];
		for (int i = 0; i < bf.MemLen; i++)
			temp.pMem[i] &= bf.pMem[i];
	}
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField bf(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) == 0u) bf.SetBit(i);
	return bf;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string string;
	istr >> string;
	TBitField temp(string.length());
	for (int i = 0; i < string.length(); i++)
		if (string[i] == '1') temp.SetBit(string.length() - 1 - i);
	bf = temp;
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}
