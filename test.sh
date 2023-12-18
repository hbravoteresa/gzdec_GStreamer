#!/bin/sh

#### Variables to configure #####
Media_Path="media"
Media_name="CarlsDate_OfficialTrailer.mp4"

#################################

gzName="$Media_Path/$Media_name.gz"

gzip -c $Media_Path\/$Media_name > $gzName

