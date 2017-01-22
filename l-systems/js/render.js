
render = function() {
    // Get l-system document
    var canvas = document.getElementById("canvas"),
        doc = document.getElementById("lsystem").textContent;
    console.log("doc: " + doc);

    var lsystem = new LSystem(doc, canvas);
    lsystem
}
