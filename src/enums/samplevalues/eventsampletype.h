#pragma once
#include <napi.h>

#define SAMPLE_EVENT_TYPE_NONE "None"
#define SAMPLE_EVENT_TYPE_DECO "Deco"
#define SAMPLE_EVENT_TYPE_RBT "RBT"
#define SAMPLE_EVENT_TYPE_ASCENT "Ascent"
#define SAMPLE_EVENT_TYPE_CEILING "Ceiling"
#define SAMPLE_EVENT_TYPE_WORKLOAD "Workload"
#define SAMPLE_EVENT_TYPE_TRANSMITTER "Transmitter"
#define SAMPLE_EVENT_TYPE_VIOLATION "Violation"
#define SAMPLE_EVENT_TYPE_BOOKMARK "Bookmark"
#define SAMPLE_EVENT_TYPE_SURFACE "Surface"
#define SAMPLE_EVENT_TYPE_SAFETY_STOP "SafetyStop"
#define SAMPLE_EVENT_TYPE_GASCHANGE "Gaschange"
#define SAMPLE_EVENT_TYPE_SAFETY_STOP_VOLUNTARY "SafetyStopVoluntary"
#define SAMPLE_EVENT_TYPE_SAFETY_STOP_MANDATORY "SafetyStopMandatory"
#define SAMPLE_EVENT_TYPE_DEEPSTOP "Deepstop"
#define SAMPLE_EVENT_TYPE_CEILING_SAFETY_STOP "CeilingSafetyStop"
#define SAMPLE_EVENT_TYPE_FLOOR "Floor"
#define SAMPLE_EVENT_TYPE_DIVETIME "Divetime"
#define SAMPLE_EVENT_TYPE_MAXDEPTH "Maxdepth"
#define SAMPLE_EVENT_TYPE_OLF "OLF"
#define SAMPLE_EVENT_TYPE_PO2 "PO2"
#define SAMPLE_EVENT_TYPE_AIRTIME "Airtime"
#define SAMPLE_EVENT_TYPE_RGBM "RGBM"
#define SAMPLE_EVENT_TYPE_HEADING "Heading"
#define SAMPLE_EVENT_TYPE_TISSUE_LEVEL_WARNING "TissueLevelWarning"
#define SAMPLE_EVENT_TYPE_GASCHANGE2 "Gaschange2"

const char *translateSampleEventType(Napi::Env env, unsigned int type);
Napi::Object getAllSampleEventTypes(Napi::Env env);