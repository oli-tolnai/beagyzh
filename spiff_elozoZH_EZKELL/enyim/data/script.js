async function calculate() {
	try {
		let num1value = document.getElementById('num1').value;
		let num2value = document.getElementById('num2').value;

		if (num1value === "" && num2value === "") {
			alert("Add meg mindkét számot!");
			return;
		}
		else if (num1value === "" && num2value !== ""){
			alert("Add meg az első számot is!");
			return;
		}
		else if (num1value !== "" && num2value === ""){
			alert("Add meg a második számot is!");
			return;
		}

		const response = await fetch("/led", {
			method: "POST",
			headers: {
				"Content-Type": "application/x-www-form-urlencoded"
			},
			// A query paraméter a POST body-ba kerül
			body: `num1=${num1value}` &&  `num2=${num2value}`
		});

		if (!response.ok) {
			throw new Error(`Hiba: ${response.status} - ${response.statusText}`);
		}

		const data = await response.text();
		// Kiírjuk a válasz szöveget
		console.log("Szerver válasza:", data);
	} catch (error) {
		// Hibakezelés
		console.error("Hiba történt:", error);
	}
}



// async function calculateOLD() {
// 	try {
// 		//érték beolvas
// 		let num1value = document.getElementById('num1').value;
// 		let num2value = document.getElementById('num2').value;

// 		if (num1value === "" && num2value === "") {
// 			alert("Add meg mindkét számot!");
// 			return;
// 		}
// 		else if (num1value === "" && num2value !== ""){
// 			alert("Add meg az első számot is!");
// 			return;
// 		}
// 		else if (num1value !== "" && num2value === ""){
// 			alert("Add meg a második számot is!");
// 			return;
// 		}

// 		//query építés
// 		let params = new URLSearchParams({ num1: num1value, num2: num2value });
// 		let url = `/calculate?${params.toString()}`;

// 		//kérés indítás
// 		let response = await fetch(url);

// 		console.log("HTTP státusz: ", response.status);

// 		//visszakapott adat jsonné konvertálása
// 		let data = await response.json();

// 		console.log("Kapott adatt:", data);

// 		//kapott adat megjelenítése
// 		document.getElementById("sumRes").innerHTML = data.sumRes;
// 		document.getElementById("subRes").innerHTML = data.mulRes;
// 		document.getElementById("divRes").innerHTML = data.mulRes;

// 	} catch (error) {
// 		console.error("Hiba történt:", error.message);
// 	}
// }