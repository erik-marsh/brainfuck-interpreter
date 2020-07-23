#pragma once

enum ErrorCode
{
    PTR_OUT_OF_RANGE_LEFT,
    PTR_OUT_OF_RANGE_RIGHT,
    UNBALANCED_BRACKETS
};

void errorMsg(const enum ErrorCode code, const char * srcFile, const long charPos);