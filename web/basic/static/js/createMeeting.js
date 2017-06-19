window.onload = function() {
    startTime = "2017-02-25/10:00";
    endTime = "2017-02-25/11:00";
    $("#createBtn").attr("disabled","true");
    $("#title").blur(function () {
        var titleObj = $(this);
        var re = /^\w+$/;
        if (!re.test(titleObj.val())) {
            var parent = titleObj.parents(".form-group");
            showError(parent);
        }
        else {
            request = createRequest();
            if (request == null) {
                alert("Unable to create request");
            }
            else {
                var theTitle = titleObj.val();
                var title = escape(theTitle);
                var url = window.location.href + "search/" + title;
                request.onreadystatechange = showTitleStatus;
                request.open("GET", url, true);
                request.send(null);
            }
        }  
    });
    $("#StartTime").blur(function () {
        var re = /^\d{4}-\d{2}-\d{2}\/\d{2}:\d{2}$/;
        var stObj = $(this);
        startTime = stObj.val();
        var parent = stObj.parents(".form-group");
        if (re.test(startTime) && isVailid(startTime)) {
            showCorrect(parent);
        }
        else {
            $("#createBtn").attr("disabled","true");
            showError(parent);
        }
    });
    $("#EndTime").blur(function () {
        var re = /^\d{4}-\d{2}-\d{2}\/\d{2}:\d{2}$/;
        var etObj = $(this);
        endTime = etObj.val();
        var parent = etObj.parents(".form-group");
        if (checkDate()) {
            showCorrect(parent);
            $("#createBtn").removeAttr("disabled");
        }
        else {
            $("#createBtn").attr("disabled","true");
            showError(parent);
        }
    });
    $("input").focus(function() {
        showDefault($(this).parents(".form-group"));
    });
}

function showTitleStatus() {
    if (request.readyState == 4) {
        if (request.status == 200) {
            var titleObj = $("#title");
            var parent = $("#title").parents(".form-group");         
            if (request.responseText == "error") {
                showCorrect(parent);
            }
            else {
                titleObj.attr("complete", "false");
                showError(parent);
                $("#createBtn").attr("disabled","true");
            }
        }
    }
}

function isVailid(timeStr) {
    var year = parseInt(timeStr.substr(0, 4));
    var month = parseInt(timeStr.substr(5, 2));
    var day = parseInt(timeStr.substr(8, 2));
    var hour = parseInt(timeStr.substr(11, 2));
    var min = parseInt(timeStr.substr(14, 2));
    return (year > 1000 && year < 3000 && month > 0 && month < 13 && day > 0 && day < 32
        && hour >= 0 && hour < 24 && min >= 0 && min < 60);
    return true;
}

function checkDate() {
    var re = /^\d{4}-\d{2}-\d{2}\/\d{2}:\d{2}$/;
    if (re.test(startTime) && re.test(endTime) && isVailid(startTime) && isVailid(endTime)) {
        var syear = parseInt(startTime.substr(0, 4));
        var smonth = parseInt(startTime.substr(5, 2));
        var sday = parseInt(startTime.substr(8, 2));
        var shour = parseInt(startTime.substr(11, 2));
        var smin = parseInt(startTime.substr(14, 2));

        var eyear = parseInt(endTime.substr(0, 4));
        var emonth = parseInt(endTime.substr(5, 2));
        var eday = parseInt(endTime.substr(8, 2));
        var ehour = parseInt(endTime.substr(11, 2));
        var emin = parseInt(endTime.substr(14, 2));

        if (eyear > syear) {
            return true;
        }
        else if (eyear < syear) {
            return false;
        }
        else {
            if (emonth > smonth) {
                return true;
            }
            else if (emonth < smonth) {
                return false;
            }
            else {
                if (eday > sday) {
                    return true;
                }
                else if (eday < sday) {
                    return false;
                }
                else {
                    if (ehour > shour) {
                        return true;
                    }
                    else if (ehour < shour) {
                        return false;
                    }
                    else {
                        if (emin > smin) {
                            return true;
                        }
                        else {
                            return false;
                        }
                    }
                }
            }
        }
    }
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