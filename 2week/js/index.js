"use strict";

const cookieName = 'userName';
const userCheck = document.getElementById('user');
const islogin = document.getElementById('login');

function getCookie(name) {
    let cookies = document.cookie.split(';');
    for(let i = 0; i < cookies.length; i++) {
        let cookie = cookies[i].trim();
        if(cookie.startsWith(name + '=')) {
            return cookie.substring(name.length + 1);
        }
    }
    return null;
}

const isCookie = getCookie(cookieName);

if (isCookie) {
    userCheck.innerHTML = isCookie + " Welcome";
    islogin.innerHTML = "Logout";
    islogin.addEventListener('click', e => {
        location.href = '/logout';
        //location.href = '/';
    })
} else {
    islogin.innerHTML = "Login";
    islogin.addEventListener('click', e => {
        location.href = '/login';
    })
}