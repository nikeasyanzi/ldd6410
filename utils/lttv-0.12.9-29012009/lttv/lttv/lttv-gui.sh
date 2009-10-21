# -* sh *-

# This is a simple script that starts lttv with default GUI modules
# Mathieu Desnoyers 15-09-2005

LTTV_CMD=`echo $0 | sed 's/-gui$//'`

$LTTV_CMD.real -m guievents -m guifilter -m guicontrolflow -m resourceview \
    -m guistatistics -m guitracecontrol $*

