async function osszeadas() {
    let numberOne =document.getElementById("numberOne").value;
    let numberTwo =document.getElementById("numberTwo").value;

    try {
    const response = await fetch("/osszeadas", {
    method: "POST",
    headers: { "Content-Type":  "application/x-www-form-urlencoded"},
    body: `szam1=${numberOne}&szam2=${numberTwo}`
    });
    if (!response.ok) {
    throw new Error(`Hiba: ${response.status} - ${response.statusText}`);
    }
    const data = await response.text();
    console.log("Szerver válasza:",data);
    } catch(error){
    console.error("Hiba történt:", error);
    }
}
async function kivonas() {
    let numberOne =document.getElementById("numberOne").value;
    let numberTwo =document.getElementById("numberTwo").value;
    try {
    const response = await fetch("/kivonas", {
    method: "POST",
    headers: { "Content-Type":  "application/x-www-form-urlencoded"},
    body: `szam1=${numberOne}&szam2=${numberTwo}`
    });
    if (!response.ok) {
    throw new Error(`Hiba: ${response.status} - ${response.statusText}`);
    }
    const data = await response.text();
    console.log("Szerver válasza:",data);
    } catch(error){
    console.error("Hiba történt:", error);
    }
}
async function osztas() {
    let numberOne =document.getElementById("numberOne").value;
    let numberTwo =document.getElementById("numberTwo").value;
    try {
    const response = await fetch("/osztas", {
    method: "POST",
    headers: { "Content-Type":  "application/x-www-form-urlencoded"},
    body: `szam1=${numberOne}&szam2=${numberTwo}`
    });
    if (!response.ok) {
    throw new Error(`Hiba: ${response.status} - ${response.statusText}`);
    }
    const data = await response.text();
    console.log("Szerver válasza:",data);
    } catch(error){
    console.error("Hiba történt:", error);
    }
}