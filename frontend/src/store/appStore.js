import {defineStore} from 'pinia'
import {ref} from 'vue'


export const useAppStore =  defineStore('app', ()=>{
    const getFrequencies = async ()=>{
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000);
        const URL = `/api/numberfrequency`;
        try {
        const response = await fetch(URL,{ method: 'GET', signal: signal });
        if (response.ok){
        const data = await response.json();
        let keys = Object.keys(data);
        if(keys.includes("status")){
        if(data["status"] == "found" ){
        console.log(data["data"] )
        return data["data"];
        }
        if(data["status"] == "failed" ){
        console.log("Inventory Item Not Found");
        }
        }
        }
        else{
        const data = await response.text();
        console.log(data);
        }
        }
        catch(err){
        console.error('getFrequencies error: ', err.message);
        }
        return []
        }	
    const getOnCount = async(LED_Name) => {
        const URL = '/api/oncount'
        // FETCH REQUEST WILL TIMEOUT AFTER 60 SECONDS
        const controller = new AbortController();
        const signal = controller.signal;
        const id = setTimeout(()=>{controller.abort()},60000);
        const form = new FormData(); // Create form
        form.append("LED_Name",LED_Name); // Add variable to form
        try {
        const response = await fetch(URL,{ method: 'POST',body:form, signal: signal });
        if(response.ok){
        const data = await response.json();
        let keys = Object.keys(data);
        // console.log(data);
        if (keys.includes("status")){
        if (data["status"] === "found"){
        console.log(data["data"]);
        return data["data"]
        }
        else if (data["status"] === "failed"){
        console.log("Unable to get LED state");
        }
        }
        }
        else{
        const data = await response.text();
        console.warn(data);
        }
        }
        catch(err){
        loading.value = false;
        if( err.message === "The user aborted a request."){
        console.log("REQUEST TIMEDOUT");
        }
        console.error('getOnCount error: ', err.message);
        }
        return 0
        }
        return {
            getOnCount,
            getFrequencies
            }
    /*  
    The composition API way of defining a Pinia store
    ref() s become state properties
    computed() s become getters
    function() s become actions  
    */ 

    // STATES 
  

//ACTIONS
   
	
	
	

    return { 
    // EXPORTS
  
       }
},{ persist: true  });