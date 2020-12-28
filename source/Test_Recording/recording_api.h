#pragma once
#ifndef RECORDING_API
#define RECORDING_API __declspec( dllexport )
#else
#define RECORDING_API __declspec( dllimport )
#endif