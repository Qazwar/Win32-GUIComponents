#include "stdafx.h"
#include "ComboBox.h"

ComboBox::ComboBox()
{
	preSelection = -1;
}

ComboBox::~ComboBox()
{
	ClearPreSelection();
}

void ComboBox::ClearPreSelection()
{
	for(auto it = preStrings.begin(); it != preStrings.end(); ++it)
		delete[] *(it);

	preStrings.clear();
}

void ComboBox::AddString(LPCWSTR string)
{
	if (GetSelf())
	{
		if (GetCount() == 0)
			SetWindowText(GetSelf(), string);

		SendMessage(GetSelf(), CB_ADDSTRING, 0, (LPARAM)string);
	}
	else
	{
		WCHAR* output = new WCHAR[wcslen(string) + 1];
		wcscpy_s(output, wcslen(string) + 1, string);
		preStrings.push_back(output);
	}
}

int ComboBox::GetSelection()
{
	if (GetSelf())
		return SendMessage(GetSelf(), CB_GETCURSEL, 0, 0);

	return 0;
}

void ComboBox::SetSelection(int index)
{
	if (GetSelf())
		SendMessage(GetSelf(), CB_SETCURSEL, (WPARAM)index, 0);
	else
		preSelection = index;
}

int ComboBox::GetCount()
{
	if (GetSelf())
		return SendMessage(GetSelf(), CB_GETCOUNT, 0, 0);
	
	return preStrings.size();
}

bool ComboBox::Create()
{
	if (GetParent() == nullptr)
		return false;

	SetSelf(CreateWindowEx(NULL, L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | WS_OVERLAPPED | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS,
				GetPosition().x, GetPosition().y, GetSize().x, GetSize().y, GetParent(), NULL, NULL, NULL));

	for (auto it = preStrings.begin(); it != preStrings.end(); ++it)
		AddString(*(it));

	if (preSelection >= 0 && preSelection < static_cast<int>(preStrings.size()))
		SetSelection(preSelection);

	ClearPreSelection();

	return GetSelf() != nullptr;
}