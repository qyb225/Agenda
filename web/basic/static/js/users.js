window.onload = initPage;

function initPage() {
    request = createRequest();
    var url = window.location.href.replace("#", "");
    url = url + "1";
    request.onreadystatechange = showPageUsers;
    request.open("GET", url, true);
    request.send(null);
    $(".pagination li").each(function () {
        $(this).click(function () {
            var url = window.location.href.replace("#", "");
            url = url + $(this).val();
            request.onreadystatechange = showPageUsers;
            request.open("GET", url, true);
            request.send(null);
        });
    });
}

function showPageUsers() {
    if (request.readyState == 4) {
        if (request.status == 200) {
            $("#userList").html(request.responseText);
        }
    }
}