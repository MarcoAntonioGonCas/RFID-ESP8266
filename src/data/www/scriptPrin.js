
const url = window.location.origin
//Ruta en donde esta ubicada nuestro servidor websock;
const urlWebsocket = `ws://${window.location.hostname}/ws`;

let socket;
//Metodo que contiene todos las operaciones con los sockets ademas de
//abrir la conexion
const reciveInfoWebsocket = (datos) => {
    let json = JSON.parse(datos);
    if (json) {

        if (json.WiFi) {
            const { CONECTADO, NOMBRE, RSSI } = json.WiFi;

            if (CONECTADO) {
                document.querySelector(".wifi-info").classList.add("wifi-info--active");
                document.querySelector("#rssiInfo").innerHTML = RSSI;

            } else {
                document.querySelector("#rssiInfo").innerHTML = "Desconectado";
                document.querySelector(".wifi-info").classList.remove("wifi-info--active");
            }
        }
    }
}

const conectarSocket = () => {
    socket = new WebSocket(urlWebsocket);
    let interval;

    function comprobarConexion() {
        socket.send("Hola");
    }
    function onOpen(e) {

        console.log(`Conectado con exito ${e}`);
    }
    function reconect() {
        console.log(`Reconectando`)
    }
    function onClose(e) {
        setTimeout(conectarSocket, 500);
    }
    function onErr(e) {
        console.log("Error en socket reconectando");
        socket.close();
    }
    function onMessage(e) {
        reciveInfoWebsocket(e.data);
    }

    socket.onopen = onOpen;
    socket.onclose = onClose;
    socket.onmessage = onMessage;
    socket.onerror = onErr;

}

conectarSocket();


//Parte de codigo que indica si se debe mostrar o cultar el menu
//desplegable esto haciendo click en la hamburguesa 
const burger = document.querySelector(".burger");
const navContainer = document.querySelector(".nav");

burger.addEventListener('click', () => {
    burger.classList.toggle("burger--active");
    navContainer.classList.toggle("nav--collapse");
});


//==========================================================
// Abrir y cerrar modales
//==========================================================
const buttonsOpenModals = document.querySelectorAll(`[data-toogle="modal"]`);
const modales = document.querySelectorAll(".modal");

buttonsOpenModals.forEach(button => {
    button.addEventListener("click",(event)=>{
        const {currentTarget} = event;
        const selector = currentTarget.dataset.target
        const modal = document.querySelector(selector);
        if(!modal)return;       
        modal.classList.add("modal--active")
        modal.style.top = `${window.scrollY}px`
        document.body.classList.add("overflow-hidden")
    })
})
modales.forEach(modal => {
    const modalClose = modal.querySelector("[data-close='modal']")
    if(!modalClose)return;

    modalClose.addEventListener("click",() =>{
        modal.classList.remove("modal--active")
        document.body.classList.remove("overflow-hidden")
    })

})