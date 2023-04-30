const url = window.location.origin;
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
        document
          .querySelector(".wifi-info")
          .classList.remove("wifi-info--active");
      }
    }
  }
};

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
    reciveInfoWebsocket(e.data);
  }

  socket.onopen = onOpen;
  socket.onclose = onClose;
  socket.onmessage = onMessage;
  socket.onerror = onErr;
};

conectarSocket();

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
  const modalClose = modal.querySelector("[data-close='modal']");
  if (!modalClose) return;

  modalClose.addEventListener("click", () => {
    modal.classList.remove("modal--active");
    document.body.classList.remove("overflow-hidden");
  });
});

//Passwoed button

const containerPassword = document.querySelectorAll(".input-password");

containerPassword.forEach((container) => {
  console.log(container);
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