const app = document.getElementById('app') as HTMLElement;

if (app) {
    app.innerHTML = `
        <h1>LEDriver Panel</h1>
        <p>Welcome to the LEDriver control panel!</p>
    `;
} else {
    console.error('App element not found');
}
