/*
 *  c.tst - Test the Appweb C API
 */

require ejs.test

let command = locate("testAppweb") + " --host " + session["host"] + " --name appweb.api.c " + test.mapVerbosity(-3)
testCmdNoCapture(command)
