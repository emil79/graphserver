#ifndef _EDGETYPES_H_
#define _EDGETYPES_H_

#include <stdlib.h>
#include <string.h>
#include "hashtable_gs.h"
#include "hashtable_itr.h"

#include "graphserver.h"
#include "statetypes.h"

//---------------DECLARATIONS FOR EDGEPAYLOAD CLASS---------------------

struct EdgePayload {
  edgepayload_t type;
  State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
  State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
} ;

EdgePayload*
epNew( edgepayload_t type, void* payload );

void
epDestroy( EdgePayload* this );

edgepayload_t
epGetType( EdgePayload* this );

State*
epWalk( EdgePayload* this, State* param, WalkOptions* options );

State*
epWalkBack( EdgePayload* this, State* param, WalkOptions* options );

//---------------DECLARATIONS FOR HEADWAYALIGHT CLASS---------------------------------------

typedef struct HeadwayAlight {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
    State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
    
    ServiceId service_id;
    char* trip_id;
    int start_time;
    int end_time;
    int headway_secs;
    
    ServiceCalendar* calendar;
    Timezone* timezone;
    int agency;
} HeadwayAlight;

HeadwayAlight*
haNew(  ServiceId service_id, ServiceCalendar* calendar, Timezone* timezone, int agency, char* trip_id, int start_time, int end_time, int headway_secs );

void
haDestroy(HeadwayAlight* this);

ServiceCalendar*
haGetCalendar( HeadwayAlight* this );

Timezone*
haGetTimezone( HeadwayAlight* this );

int
haGetAgency( HeadwayAlight* this );

ServiceId
haGetServiceId( HeadwayAlight* this );

char*
haGetTripId( HeadwayAlight* this );

int
haGetStartTime( HeadwayAlight* this );

int
haGetEndTime( HeadwayAlight* this );

int
haGetHeadwaySecs( HeadwayAlight* this );

inline State*
haWalk( EdgePayload* superthis, State* state, WalkOptions* options );

inline State*
haWalkBack( EdgePayload* superthis, State* state, WalkOptions* options );

//---------------DECLARATIONS FOR CROSSING CLASS-------------------------------------------

typedef struct Crossing {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
    State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
    
    int* crossing_times;
    char** crossing_time_trip_ids;
    int n;
} Crossing;

Crossing*
crNew( );

void
crDestroy(Crossing* this);

void
crAddCrossingTime(Crossing* this, char* trip_id, int crossing_time);

int
crGetCrossingTime(Crossing* this, char* trip_id);

char*
crGetCrossingTimeTripIdByIndex(Crossing* this, int i);

int
crGetCrossingTimeByIndex(Crossing* this, int i);

int
crGetSize(Crossing* this);

inline State*
crWalk( EdgePayload* superthis, State* state, WalkOptions* options );

inline State*
crWalkBack( EdgePayload* superthis, State* state, WalkOptions* options );

//---------------DECLARATIONS FOR ALIGHT CLASS---------------------------------------------

typedef struct Alight {
    edgepayload_t type;
    State* (*walk)(struct EdgePayload*, struct State*, struct WalkOptions*);
    State* (*walkBack)(struct EdgePayload*, struct State*, struct WalkOptions*);
    
    int n;
    int* arrivals;
    char** trip_ids;
    int* stop_sequences;
    
    ServiceCalendar* calendar;
    Timezone* timezone;
    int agency;
    ServiceId service_id;
    
    int overage; //number of seconds schedules past midnight of the last departure. If it's at 12:00:00, the overage is 0.
} Alight;

Alight*
alNew( ServiceId service_id, ServiceCalendar* calendar, Timezone* timezone, int agency );

void
alDestroy(Alight* this);

ServiceCalendar*
alGetCalendar( Alight* this );

Timezone*
alGetTimezone( Alight* this );

int
alGetAgency( Alight* this );

ServiceId
alGetServiceId( Alight* this );

int
alGetNumAlightings(Alight* this);

void
alAddAlighting(Alight* this, char* trip_id, int arrival, int stop_sequence);

char*
alGetAlightingTripId(Alight* this, int i);

int
alGetAlightingArrival(Alight* this, int i);

int
alGetAlightingStopSequence(Alight* this, int i);

int
alSearchAlightingsList(Alight* this, int time);

int
alGetLastAlightingIndex(Alight* this, int time);

int
alGetOverage(Alight* this);

int
alGetAlightingIndexByTripId(Alight* this, char* trip_id);

inline State*
alWalk(EdgePayload* this, State* state, WalkOptions* options);

inline State*
alWalkBack(EdgePayload* this, State* state, WalkOptions* options);

typedef struct PayloadMethods {
	void (*destroy)(void*);
	State* (*walk)(void*,State*,WalkOptions*);
	State* (*walkBack)(void*,State*,WalkOptions*);
	//char* (*to_str)(void*);
} PayloadMethods;

typedef struct CustomPayload {
  edgepayload_t type;
  void* soul;
  PayloadMethods* methods;
} CustomPayload;

PayloadMethods*
defineCustomPayloadType(void (*destroy)(void*),
						State* (*walk)(void*,State*,WalkOptions*),
						State* (*walkback)(void*,State*,WalkOptions*));


void
undefineCustomPayloadType( PayloadMethods* this );

CustomPayload*
cpNew( void* soul, PayloadMethods* methods );

void
cpDestroy( CustomPayload* this );

void*
cpSoul( CustomPayload* this );

PayloadMethods*
cpMethods( CustomPayload* this );

State*
cpWalk(CustomPayload* this, State* state, struct WalkOptions* walkoptions);

State*
cpWalkBack(CustomPayload* this, State* state, struct WalkOptions* walkoptions);

#endif
