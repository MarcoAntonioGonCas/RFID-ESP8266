
// const rutaApi = `${url}/api/redes`
const rutaApi = `http://192.168.0.109/api/redes`
const container = document.querySelector("#container-redes")
const btnSearch = document.querySelector("#btnBuscarRedes");
let redes = [];


btnSearch.addEventListener("click", () =>{
    toogleButtonSearch(true);
    llamaApi()
});

const selecWifi = (button)=>{
    const {id:index} = button.dataset 
    const {ssid,enc} = redes[index]

    document.querySelector("#txtNombreRed").value = ssid;

    
    document.querySelector("#txtContraRed").value="";
    document.querySelector("#txtContraRed").disabled = enc=="";

    document.querySelector("[data-close='modal']").click()
}


container.addEventListener("click",e=>{
    const {target} = e
    if(target.tagName == "BUTTON"){
        selecWifi(target)
    }
})


const toogleButtonSearch= (disabled = true)=>{
    if(disabled){
        btnSearch.classList.add("buscando")
    }else{
        btnSearch.classList.remove("buscando")
    }
    btnSearch.disabled = disabled
}


const borraNodos = (element)=>{ 
    while(element.firstElementChild){
        element.removeChild(element.firstElementChild)
    }
    element.innerHTML="";
}
const agregaFila = ({id,ssid,rssi,enc,channel})=>{
    const template = `
       <tr>
            <td>${id+1}</td>
            <td>${ssid}</td>
            <td>${rssi}</td>
            <td>${enc}</td>
            <td>${channel}</td>
            <td>
                <button class="btn btn-success" data-id=${id}>Conectar</button>
            </td>
        </tr>
    `;
    
    container.innerHTML += template;
    toogleButtonSearch(false)
}

const compruebajson = (json)=>{
    if(json.count > 0){

        redes = json.redes;

        for(let i =0;i<redes?.length;i++){

            let red = redes[i];

            agregaFila({
                id:i,
                ...red
            })

        }
    }else{
        setTimeout(5000,llamaApi)
    }
}

const realizaPeticion = (url) => {
    borraNodos(container);

    fetch(rutaApi)
    .then(res => res.json())
    .then(compruebajson)
    .catch(err =>{
        alert("Error al buscar redes")
        console.log(err);
        toogleButtonSearch(false)
    })
}

function llamaApi (){
    realizaPeticion(rutaApi)
};

// Proxy





const habilitarProxy = document.getElementById("proxy-habilitado");
const txtProxy = document.getElementById("proxy");
const txtPuerto = document.getElementById("puerto");

habilitarProxy.addEventListener("change",e=>{
    
    
    if(habilitarProxy.checked){
        txtProxy.disabled = false;
        txtPuerto.disabled = false;
        
    }else{
        txtProxy.disabled = true;
        txtPuerto.disabled = true;
    }
});
