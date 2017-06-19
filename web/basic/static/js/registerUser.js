window.onload = initPage;

function checkComplete() {
    //$("#regBtn").removeAttr("disabled");
    if ($("#username").attr("complete") == "true"
            && $("#password").attr("complete") == "true"
            && $("#passwordAgain").attr("complete") == "true"
            && $("#telephone").attr("complete") == "true"
            && $("#email").attr("complete") == "true") {
        $("#regBtn").removeAttr("disabled");
    }
    else {
        $("#regBtn").attr("disabled","true");
    }
}

function initPage() {
    $("#regBtn").attr("disabled","true");
    $("#username").blur(checkUserUsername).attr("complete", "flase");
    $("#password").blur(checkUserPassword).attr("complete", "flase");
    $("#passwordAgain").blur(checkUserPasswordAgain).attr("complete", "flase");
    $("#telephone").blur(checkUserTel).attr("complete", "true");
    $("#email").blur(checkUserEmail).attr("complete", "true");

    $("input").focus(function() {
        $(this).attr("complete", "false");
        showDefault($(this).parents(".form-group"));
        checkComplete();
    });
}

/*Username*/
function checkUserUsername() {
    var usernameObj = $("#username");
    usernameObj.attr("complete", "false");
    request = createRequest();
    if (request == null) {
        alert("Unable to create request");
    }
    else {
        var theName = usernameObj.val();
        var username = escape(theName);

        if (checkUsernameRe(username)) {
            var url = window.location.href + username;
            request.onreadystatechange = showUsernameStatus;
            request.open("GET", url, true);
            request.send(null);
        }
        else {
            var parent = usernameObj.parents(".form-group");
            showError(parent);
        }
    }
}

function checkUsernameRe(username) {
    var re = /^[a-zA-z]\w{5,15}$/;
    return re.test(username);
}

function showUsernameStatus() {
    if (request.readyState == 4) {
        if (request.status == 200) {
            var usernameObj = $("#username");
            var parent = $("#username").parents(".form-group");         
            if (request.responseText == "error") {
                usernameObj.attr("complete", "true");
                showCorrect(parent);
            }
            else {
                usernameObj.attr("complete", "false");
                showError(parent);   
            }
            checkComplete();
        }
    }
}

/*Password*/
function checkUserPassword() {
    var passwordObj = $("#password");
    passwordObj.attr("complete", "false");
    var password = passwordObj.val();
    var re = /^\S{5,20}$/;
    var parent = passwordObj.parents(".form-group");
    if (re.test(password)) {
        passwordObj.attr("complete", "true");
        showCorrect(parent);
    }
    else {
        showError(parent);
    }
    checkComplete();
}

function checkUserPasswordAgain() {
    var passwordAgainObj = $("#passwordAgain");
    passwordAgainObj.attr("complete", "false");
    var password = $("#password").val();
    var passwordAgain = passwordAgainObj.val();
    var re = /^\S{5,20}$/;
    var parent = passwordAgainObj.parents(".form-group");
    if (passwordAgain == password && re.test(passwordAgain)) {
        passwordAgainObj.attr("complete", "true");
        showCorrect(parent);
    }
    else {
        showError(parent);
    }
    checkComplete();
}

/*Id*/
function checkUserEmail() {
    var idObj = $("#email");
    idObj.attr("complete", "false");
    var idNum = idObj.val();
    var parent = idObj.parents(".form-group");
    var reIdNum = /^([a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+(.[a-zA-Z0-9_-])+/;
    if (reIdNum.test(idNum)) {
        idObj.attr("complete", "true");
        showCorrect(parent);
    }
    else {
        showError(parent);
    }
    checkComplete();
}


/*Tel*/
function checkUserTel() {
    var telObj = $("#telephone");
    telObj.attr("complete", "false");
    var telNum = telObj.val();
    var reMobile=/^1[3,5,7,8]\d{9}$/;
    var parent = telObj.parents(".form-group");
    if (reMobile.test(telNum)) {
        showCorrect(parent);
        telObj.attr("complete", "true");
    }
    else {
        showError(parent);
    }
    checkComplete();
}


function showCorrect(parent) {
    parent.removeClass("has-error").addClass("has-success");
    parent.find("span").removeClass("glyphicon-remove").addClass("glyphicon-ok");
}

function showError(parent) {
    parent.removeClass("has-success").addClass("has-error");
    parent.find("span").removeClass("glyphicon-ok").addClass("glyphicon-remove");
}

function showDefault(parent) {
    parent.removeClass("has-error").removeClass("has-success");
    parent.find("span").removeClass("glyphicon-ok").removeClass("glyphicon-remove");
}