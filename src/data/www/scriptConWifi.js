
const rutaApi = `${url}/api/redes`
// const rutaApi = `http://192.168.0.107/api/redes`
const container = document.querySelector("#container-redes")
const btnSearch = document.querySelector("#btnBuscarRedes");
let redesAux = [];
let id;



const onBotonSeleccionaRed = (button)=>{
    const {id:index} = button.dataset 
    const {ssid,enc} = redesAux[index]

    document.querySelector("#txtNombreRed").value = ssid;

    
    document.querySelector("#txtContraRed").value="";
    document.querySelector("#txtContraRed").disabled = enc=="";

    document.querySelector("[data-close='modal']").click()
}


const toogleButtonSearchDisabled= (disabled = true)=>{
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


//peticion
const agregaFila = ({id,ssid,rssi,bssid,enc,channel})=>{
    const template = `
       <tr>
            <td>${id+1}</td>
            <td>${ssid}</td>
            <td>${bssid}</td>
            <td>${rssi}</td>
            <td>${enc}</td>
            <td>${channel}</td>
            <td>
                <button class="btn btn-success" data-id=${id}>Conectar</button>
            </td>
        </tr>
    `;
    
    container.innerHTML += template;
}

const compruebajson = (json)=>{

    const {count = 0,redes } = json
    if(count > 0){
        for(let i = 0; i < redes?.length; i++){
            let red = redes[i];

            agregaFila({
                id:i,
                ...red
            })

        }
        toogleButtonSearchDisabled(false);
        redesAux = redes;
    }else{
        setTimeout(llamaApi,5000)
    }
}

const realizaPeticion = (url) => {
    borraNodos(container);

    fetch(url)
    .then(res => res.json())
    .then(compruebajson)
    .catch(err =>{
        if(err.name == "AbortError"){
            alert("Tiempo de espera agotado")
        }else{
            alert("Error al buscar redes")

        }
        console.log(err);
        toogleButtonSearchDisabled(false)
    })
}
 

function llamaApi (){
    realizaPeticion(rutaApi)

};



btnSearch.addEventListener("click", () =>{
    toogleButtonSearchDisabled(true);
    llamaApi()
});
container.addEventListener("click",e=>{
    const {target} = e
    if(target.tagName == "BUTTON"){
        onBotonSeleccionaRed(target)
    }
})


// --------------------------------------------------------
// Seccion proxy
// --------------------------------------------------------
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
