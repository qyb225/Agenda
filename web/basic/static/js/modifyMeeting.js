window.onload = initPage;

function initPage() {
    $("#deleteBtn").attr("href", window.location.href + "delete/");
    $("#partAdder").focus(function () {
        $("#addFeedback").html("");
    });
    $("#partAdderBtn").click(function () {
        request = createRequest();
        if (request == null) {
            alert("Unable to create request");
        }
        else {
            var part = $("#partAdder").val();
            var partName = escape(part);
            var url = window.location.href + "add/" + partName;
            request.onreadystatechange = showAddStatus;
            request.open("GET", url, true);
            request.send(null);
        }
    });
}

function showAddStatus() {
    if (request.readyState == 4) {
        if (request.status == 200) {
            var resText = request.responseText;
            if (resText != "error") {
                addPartOnPage(resText);
                $("#partAdder").val("");
            }
            else {
                $("#addFeedback").html("Cannot add");
            }
        }
    }
}

function addPartOnPage(partInfo) {
    var partInfoJson = eval( '(' + partInfo + ')');
    $("#participator table").append("<tr><td><span class='glyphicon glyphicon-user'></span> " 
        + partInfoJson.username + "</td><td>" + partInfoJson.telephone + "</td></tr>");
}