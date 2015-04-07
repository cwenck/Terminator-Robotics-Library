/*
 * Util.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: guywenck
 */

#include "TRL/Util.h"

namespace TRL {

float getMainBatteryPower() {
	return ((float) powerLevelMain()) / 1000.0f;
}

/*
 * Get a string with allocated memory for a specific number of chars
 *
 * Assuming exampleStrPtr is a string* its memory of *exampleStrPtr should be freed
 * after you are done with the string with free(*exampleStrPtr) or if it is only a string exampleStr
 * the memory should be freed with free(exampleStr).
 *
 * @param the number of chars before the null terminator to allocate memory for
 */
string allocateStringForNumberOfChars(int numChars) {
	//add 1 to account for the null terminator
	return new char[numChars + 1];
//	return (string) malloc(sizeof(char) * (numChars + 1));
}

/*
 * Get the substring of a bigger string with the start index startChar.
 * The index is zero indexed and startChar is inclusive.
 *
 * Assuming exampleStrPtr is a string* its memory of *exampleStrPtr should be freed
 * after you are done with the string with free(*exampleStrPtr) or if it is only a string exampleStr
 * the memory should be freed with free(exampleStr).
 *
 * @param storeStr is the string the substring will be stored in
 * @param str is the string that a substring will be taken of
 * @param startChar is the index of the first char that will be included
 */
string strsub(string* storeStr, string str, int startChar) {
	return strsub(storeStr, str, startChar, strlen(str));
}

/*
 * Get the substring of a bigger string with the start index startChar.
 * The index is zero indexed, startChar is inclusive, endChar is exclusive.
 *
 * Assuming exampleStrPtr is a string* its memory of *exampleStrPtr should be freed
 * after you are done with the string with free(*exampleStrPtr) or if it is only a string exampleStr
 * the memory should be freed with free(exampleStr).
 *
 * @param storeStr is the string the substring will be stored in
 * @param str is the string that a substring will be taken of
 * @param startChar is the index of the first char that will be included
 * @param endChar is the index of the first char that will not be included
 */
string strsub(string* storeStr, string str, int startChar, int endChar) {
	int subStrLen = endChar - startChar;
	*storeStr = allocateStringForNumberOfChars(subStrLen);

	char* cursor = (char*) *storeStr;
	for (int i = startChar; i < endChar; i++) {
		*cursor = str[i];
		cursor++;
	}
	*cursor = '\0';
	return *storeStr;
}

} /* namespace TRL */
