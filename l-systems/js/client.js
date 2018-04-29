/* Convert the L-System rule into a document
   Query the server which will do it in the Python code
*/

GenerateDocument = function () {
    var rule = document.getElementById("lsystem_rule"),
        gen = document.getElementById("lsystem_generations"),
        doc = document.getElementById("lsystem_document");

    var req = new XMLHttpRequest();
    req.onload = function (response) {
        data = response.currentTarget.response;
        data = JSON.parse(data)
        console.log(data);
        doc.textContent = data['doc'];
    };

    req.open('POST', 'http://localhost:8080/api', true);
    req.send(JSON.stringify({
        rule: rule.value,
        generations: gen.value
    }));
}

