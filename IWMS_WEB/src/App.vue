<template>
  <div id="app">
    <!-- 仓库列表组件 -->
    <warehouse-list></warehouse-list>
    <!-- 路由视图组件 -->
    <router-view></router-view>
  </div>
</template>

<script>
import axios from "axios"

export default {
  name: 'App',
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
