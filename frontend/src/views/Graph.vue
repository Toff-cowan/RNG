<template>
  <v-container class="container"> 
      <v-row class="row" justify="center">
          <v-col class="col col1" align="center" cols="12"> 
              <canvas id="myChart"></canvas>     
          </v-col>
          <v-col class="col col2" align="center" cols="12"> 
              <v-btn  class="text-caption" text="Refresh Graph" variant="outlined" color="secondary" flat  @click="updateGraph()"></v-btn>
          </v-col>

      </v-row>

      <v-row class="row" >
          <v-divider class="mt-5 mb-5"></v-divider>
      </v-row>
      <v-row class="row" justify="center">
          
          <v-card class="ma-2" subtitle="LED A" width="150" align="center" border flat > 
              <VCardItem class="pa-0" >Turned on </VCardItem>     
              <VCardItem class="pa-0"><span class="text-h5 text-primary font-weight-bold">{{led_A}}</span> </VCardItem>  
              <VCardItem class="pa-0"> times</VCardItem>        
              <VCardItem> 
                  <VBtn text="Update" class="ma-1 text-caption" rounded="pill" flat color="secondary" variant="tonal" @click="updateLEDCount('ledA')"></VBtn> 
              </VCardItem> 
          </v-card>
          <v-card class="ma-2" subtitle="LED B" width="150" align="center" border flat > 
              <VCardItem class="pa-0" >Turned on </VCardItem>     
              <VCardItem class="pa-0"><span class="text-h5 text-primary font-weight-bold">{{ led_B}}</span> </VCardItem>  
              <VCardItem class="pa-0"> times</VCardItem>        
              <VCardItem> 
                  <VBtn text="Update" class="ma-1 text-caption" rounded="pill" flat color="secondary" variant="tonal" @click="updateLEDCount('ledB')"></VBtn> 
              </VCardItem> 
          </v-card>
      </v-row>

  </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useAppStore } from "@/store/appStore"; 

import Chart from 'chart.js/auto'; 
const AppStore    = useAppStore(); 


// VARIABLES
const router      = useRouter();
const route       = useRoute();  
const led_A       = ref(0); // Store count for LED A 
const led_B       = ref(0); // Store count for LED B 
let chart         = null;  // Chart object 
const data        = { labels:  ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'], 
                  datasets: [ 
                      { 
                          label: 'Fully Rounded', 
                          data: [0, 0, 0, 0, 0, 0], 
                          borderColor: '#1ECBE1', 
                          backgroundColor: '#4BD5E7', 
                          borderWidth: 2, 
                          borderRadius: 5, 
                          borderSkipped: false, 
}] 
}; 
const config      = { type: 'bar', 
                     data: data, 
                     options: { 
                      responsive: true, 
                      plugins: { 
                      legend: { 
                      position: 'top', }, 
                  title: { 
                      display: true, 
                      text: 'Chart.js Bar Chart' 
                  } 
                      } 
}, 
}; 

// FUNCTIONS
onMounted(()=>{
  // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
  const ctx = document.querySelector('#myChart'); // Select canvas for rendering chart 
  chart = new Chart(ctx, config ); // create chart 
});


onBeforeUnmount(()=>{
  // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
});

// Update graph with labels and new data 
const updateData = ( chart, label, newData) => { 
  chart.data.labels = label; 
  chart.data.datasets[0].data = newData; 
  chart.update();
}


// FUNCTIONS  

// Fetch new data and update graph
const updateGraph = async () =>{ 
  let result = await AppStore.getFrequencies(); 
  let labels = []; 
  let data   = []; 

  if (result.length > 0){
      result.forEach(obj => { 
          labels.push(obj["number"]) 
          data.push(obj["frequency"]) 
      });
      updateData(chart,labels,data); 
  }
}


// Fetch new data and update cards 
const updateLEDCount = async(name)=>{ 
  let result =  await AppStore.getOnCount(name); 
  // console.log(result); 
  if (name == "led_A"){ 
      led_A.value = result; 
  } 
  if (name == "led_B"){ 
      led_B.value = result; 
  }
}







</script>


<style scoped>
/** CSS STYLE HERE */
/* Make the container fluid */
.container {
  width: 90%; /* Fluid width */
  max-width: 1200px; /* Set a max width for larger screens */
  margin: 0 auto; /* Centers the container horizontally */
  text-align: center; /* Centers text inside the container */
  padding: 20px;
  background-color: lightgray;
  border-radius: 10px;
}
.row{
  max-width: 1200px;
}
</style>