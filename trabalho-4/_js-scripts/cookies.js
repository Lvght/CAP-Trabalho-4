

// veriifica se o cookie de login existe
function isLoggedIn() {
    if (getCookie("loggedin") == undefined) {
        alert("Não fez login!");
    } else {
        alert("O login foi validado!");
    }
}

// retorna o cookie de nome especificado
function getCookie(name) {
    var value = "; " + document.cookie;
    var parts = value.split("; " + name + "=");
    if (parts.length == 2) return parts.pop().split(";").shift();
}

function destroyCookie(name) {
    document.cookie = name + "= ; expires = Thu, 01 Jan 1970 00:00:00 GMT"
}



// para fins de teste : define o login como verdadeiro
function setLogin() {
    document.cookie = "loggedin=1";
}

// para fins de teste : define o login como falso
function unsetLogin() {
    document.cookie = "loggedin= ; expires = Thu, 01 Jan 1970 00:00:00 GMT"
}

function moveWithCookies(path) {
    var usrname = getCookie("usrname");
    var pass = getCookie("pin");
}