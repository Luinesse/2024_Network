document.addEventListener('DOMContentLoaded', function() {
    let cookieName = 'userName';
    const loginBtn = document.getElementById('btn');

    loginBtn.addEventListener('click', function(event) {
        event.preventDefault();

        window.location.href = "/";
    });
});