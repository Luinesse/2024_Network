document.addEventListener('DOMContentLoaded', function() {
    let cookieName = 'userName';
    const loginBtn = document.getElementById('btn');

    loginBtn.addEventListener('click', function(event) {
        event.preventDefault();

        let userName = document.getElementById('userName').value;
        let expDate = new Date();
        expDate.setDate(expDate.getDate() + 1);

        const data = {
            userName: userName
        };

        fetch('/', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(data)
        })
        window.location.href = "/";
    });
});