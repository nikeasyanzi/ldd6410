# -* sh *-

# This is a simple script that starts lttv with no modules :
# For batch mode.
# Mathieu Desnoyers 15-09-2005

if [ x"$*" = x"" ]; then
  echo "This is a wrapper around $0.real for convenience purposes"
  echo "What you really want is maybe the lttv-gui command ?"
  echo
  $0.real --help
else
  $0.real $*
fi
