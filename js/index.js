function use_current_year() {
	document.querySelector("#current_year").textContent = new Date().getFullYear().toString();
}

window.addEventListener('DOMContentLoaded', (evt) => {
	use_current_year();
});
