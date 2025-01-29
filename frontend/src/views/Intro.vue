<template>
    <div>
        <!-- HTML HERE -->    
        <h2>New Template</h2>
    </div>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  


// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
  mqttStore.subscribe("esp32/randomNumber");
  mqttStore.subscribe("esp32/ledState");
  mqttStore.setMessageHandler(handleMQTTMessage);
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    mqttStore.unsubscribe("esp32/randomNumber");
    mqttStore.unsubscribe("esp32/ledState");
});


</script>


<style scoped>
/** CSS STYLE HERE */

.v-card {
    text-align: center;
}

</style>
   