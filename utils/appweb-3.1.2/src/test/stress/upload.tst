/*
 *  upload.tst - Stress test uploads
 */

require ejs.test

const HTTP = session["main"]
const URL = HTTP + "/index.html"
const EURL = HTTP + "/index.ejs"
const TESTFILE = "stress/upload-" + hashcode(self) + ".tdat"

let http: Http = new Http

/* Depths:    0  1  2  3   4   5   6    7    8    9    */
var sizes = [ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 ]

//
//  Create test data 
//
buf = new ByteArray
for (i in 64) {
    for (j in 15) {
        buf.writeByte("A".charCodeAt(0) + (j % 26))
    }
    buf.writeByte("\n".charCodeAt(0))
}

//
//  Create test data file
//
f = File(TESTFILE).open({mode: "w"})
for (i in (sizes[test.depth] * 1024)) {
    f.write(buf)
}
f.close
f.open()

if (test.threads == 1) {
    size = Path(TESTFILE).size

    http.upload(HTTP + "/upload.ejs", { file: TESTFILE })
    assert(http.code == 200)

    let uploaded = Path("web/tmp").join(Path(TESTFILE).basename)
    assert(uploaded.size == size)
    sh("diff " + uploaded + " " + TESTFILE)
}
rm(TESTFILE)
