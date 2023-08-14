const url = window.location.origin;
//Ruta en donde esta ubicada nuestro servidor websock;
const urlWebsocket = `ws://${window.location.hostname}/ws`;
// const urlWebsocket = `ws://192.168.0.107/ws`;
let socket;


// Metodo que contiene todos las operaciones con los sockets ademas de
// abrir la conexion

const callbackJSON = [];
const cardWifi    = document.querySelector("#card-wifi");
const labelWifi   = cardWifi.querySelector("label");
const imgWifi     = cardWifi.querySelector("img");
const labelIPWifi = document.querySelector("#info-wifi");


const limpiarClasesWifi = (element) =>{
  element.classList.remove("label-danger");
  element.classList.remove("label-info");
  element.classList.remove("label-success");
  element.classList.remove("label-danger");
}

const obtenerClaseWifi = (senial) =>{
  if(senial == 0){
    return "label-danger"
  }else if(senial == 1){
    return "label-info"
  }else if(senial == 2 || senial == 3){
    return "label-success"
  }else{
    return "label-danger"
  }
}

const reciveInfoWsWifi = (json) => {
  const {WiFi,ESP} = json;
  if (!WiFi)return; 

  const { CONECTADO, NOMBRE, RSSI,IP } = WiFi;
  
  if(labelIPWifi){
    labelIPWifi.textContent = `IP: ${IP}`
  }

  if (CONECTADO) {
    const senial = Math.ceil( map(RSSI, -40, -80, 0, 3) );    
    const clase = obtenerClaseWifi(senial);

    cardWifi.querySelector(".item-status").classList.add("item-status--active");
    
    labelWifi.innerHTML = RSSI;
    limpiarClasesWifi(labelWifi);
    labelWifi.classList.add(clase);

    if(imgWifi.src != `www/wifi-${senial}.svg`){
      imgWifi.src = `www/wifi-${senial}.svg`
    }

    

  } else {

    
    labelWifi.innerHTML = "Desconectado";
    cardWifi.querySelector(".item-status").classList.remove("item-status--active");
    labelWifi.classList.add("label-danger");
  }
  

};

// Punto de acceso
const cardAp = document.querySelector("#card-ap");
const labelAP = cardAp.querySelector("label")

const reciveInfoWsAp = (json) =>{
  const {AP} = json;
  if(!AP)return;

  const {HABI,CLIEN} = AP;


  if(HABI){
    cardAp.querySelector(".item-status").classList.add("item-status--active");
    labelAP.innerHTML = `conectados: ${CLIEN}`
  }else{
    
    cardAp.querySelector(".item-status").classList.remove("item-status--active");
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
    console.log(`Reconectando`);
  }
  function onClose(e) {
    setTimeout(conectarSocket, 500);
  }
  function onErr(e) {
    console.log("Error en socket reconectando");
    socket.close();
  }
  function onMessage(e) {

    const json = JSON.parse(e.data);

    if(!json)return;

    callbackJSON.forEach(metodo =>{
      metodo(json);
    })
  }

  socket.onopen = onOpen;
  socket.onclose = onClose;
  socket.onmessage = onMessage;
  socket.onerror = onErr;
};

conectarSocket();
callbackJSON.push(reciveInfoWsWifi,reciveInfoWsAp);
//Parte de codigo que indica si se debe mostrar o cultar el menu
//desplegable esto haciendo click en la hamburguesa
const burger = document.querySelector(".burger");
const navContainer = document.querySelector(".nav");
const navsdropdown = document.querySelectorAll(".nav-dropdown");

const toggleHeight = (element) => {
  let tamanio = 0;

  if (element.clientHeight == 0) {
    tamanio = element.scrollHeight;
  } else {
    tamanio = 0;
  }
  element.style.height = `${tamanio}px`;
};

navsdropdown.forEach((drop) => {
    drop.addEventListener("click", () => {
            if (navContainer.classList.contains("nav--collapse")) {
                burger.classList.remove("burger--active");
                navContainer.classList.remove("nav--collapse");
            }
            drop.classList.toggle("nav-dropdown--active");
            toggleHeight(drop.lastElementChild);
        });
});

const comprobarCollapso = () => {
    if (!navContainer.classList.contains("nav--collapse")) {
        navsdropdown.forEach((drop) => {
        if (drop.classList.contains("nav-dropdown--active")) {
            drop.classList.remove("nav-dropdown--active");
            toggleHeight(drop.lastElementChild);
        }
    });
    }
};

burger?.addEventListener("click", () => {
    comprobarCollapso();
    burger.classList.toggle("burger--active");
    navContainer.classList.toggle("nav--collapse");
});
comprobarCollapso();

//==========================================================
// Abrir y cerrar modales
//==========================================================
const buttonsOpenModals = document.querySelectorAll(`[data-toogle="modal"]`);
const modales = document.querySelectorAll(".modal");

buttonsOpenModals.forEach((button) => {
  button.addEventListener("click", (event) => {
    const { currentTarget } = event;
    const selector = currentTarget.dataset.target;
    const modal = document.querySelector(selector);
    if (!modal) return;
    modal.classList.add("modal--active");
    modal.style.top = `${window.scrollY}px`;
    document.body.classList.add("overflow-hidden");
  });
});


modales.forEach((modal) => {
  modal.addEventListener("click",e=>{
    const {dataset } = e.target;
    if(dataset.close == "modal"){
      modal.classList.remove("modal--active");
    document.body.classList.remove("overflow-hidden");
    }
  })
});


// Boton contraseña
const containerPassword = document.querySelectorAll(".input-password");

containerPassword.forEach((container) => {
  const btn = container.querySelector("[data-toogle]");
  const input = container.querySelector(btn.dataset.toogle);
  btn.addEventListener("click", () => {
    if (input) {
      if (input.type == "text") {
        input.type = "password";
      } else {
        input.type = "text";
      }
    }
  });
});


//TODO: Metodo para enviar datos
const sendData = (selectorFrm) =>{
  const frm = document.querySelector(selectorFrm);
  if(!frm)return;
  const {action,method} = frm;

  frm.addEventListener("submit",e=>{
    e.preventDefault();
    const url = new URL(action);
    url.host = "192.168.0.106";
    url.port=""
    
    const frmData = new FormData(frm);
    const dataParams = new URLSearchParams(new FormData(frm));
    console.log(dataParams.toString());


    const jsonData = {};

    for (var [k, v] of frmData) {
      jsonData[k] = v;
    }
    
    fetch(
      url,{
      method,
      headers: {
        //'Content-Type': 'application/json'
         'Content-Type': 'application/x-www-form-urlencoded'
      },
      body:dataParams
    }).then(e=>{
      console.log(e);
    }).catch(err=>{
      location.reload();
    })
  })



}

// sendData("#frm");



// Helpers

function map(value,valueMin,valueMax,newValueMin,newValueMax){
  return  ( value - valueMin) * (newValueMax - newValueMin) / (valueMax - valueMin) + newValueMin
}