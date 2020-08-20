<template>
  <div id="app">
    <warehouse-list></warehouse-list>
    <router-view></router-view>
  </div>
</template>

<script>
import axios from "axios"

export default {
  name: 'App',
  methods: {
    /**
      onGetClick() {
        axios.get('/cgi-bin/test.cgi/?ID=12345')
        .then(function (response) {
          console.log(response);
        })
        .catch(function (error) {
          console.log(error);
        });
      },
      onPostClick() {
        axios.post('/cgi-bin/test.cgi', {
          firstName: 'Fred',
          lastName: 'Flintstone'
        })
        .then(function (response) {
          console.log(response);
        })
        .catch(function (error) {
          console.log(error);
        });
      }
    */
  }
}
</script>

<style lang="scss">
html, body {
  height: 100%;
}
#app {
  background-color: #fafafb;
  height: 100%;
  min-width: 1700px;
  width: 100%;
}
</style>
