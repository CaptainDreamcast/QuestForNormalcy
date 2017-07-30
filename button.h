#pragma once

void loadButtons();

int addButton();
void removeButton(int tID);

int hasPressedButton(int tID);
int isButtonBelowRange(int tID);

void setButtonSpeed(double x);