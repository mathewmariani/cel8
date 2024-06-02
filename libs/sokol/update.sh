#!/bin/bash
# update the required sokol headers

# repo url
SOKOL_PREFIX="https://raw.githubusercontent.com/floooh/sokol/master"

# headers
SOKOL_APP_H="$SOKOL_PREFIX/sokol_app.h"
SOKOL_GFX_H="$SOKOL_PREFIX/sokol_gfx.h"
SOKOL_GLUE_H="$SOKOL_PREFIX/sokol_glue.h"
SOKOL_LOG_H="$SOKOL_PREFIX/sokol_log.h"
SOKOL_TIME_H="$SOKOL_PREFIX/sokol_time.h"

# license
SOKOL_LICENSE="$SOKOL_PREFIX/LICENSE"

# output directory
OUTPUT=./libs/sokol

# curl all headers and utils
curl $SOKOL_APP_H > $OUTPUT/sokol_app.h
curl $SOKOL_GFX_H > $OUTPUT/sokol_gfx.h
curl $SOKOL_GLUE_H > $OUTPUT/sokol_glue.h
curl $SOKOL_LOG_H > $OUTPUT/sokol_log.h
curl $SOKOL_TIME_H > $OUTPUT/sokol_time.h

# license
curl $SOKOL_LICENSE > $OUTPUT/LICENSE