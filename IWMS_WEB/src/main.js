// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import VCharts from 'v-charts'
import ViewUI from 'view-design'
import store from "./store/store.js"

import "@/style/reset.scss"
import 'view-design/dist/styles/iview.css'
import "@/assets/font/iconfont.css"


// 组件注册
import WarehouseList from "@/components/WarehouseList.vue";
Vue.component(WarehouseList.name, WarehouseList);
import WarehousesInfo from "@/components/WarehousesInfo.vue";
Vue.component(WarehousesInfo.name, WarehousesInfo);
import HistoryStatistics from "@/components/HistoryStatistics.vue";
Vue.component(HistoryStatistics.name, HistoryStatistics);
import WarehouseMaintenance from "@/components/WarehouseMaintenance.vue";
Vue.component(WarehouseMaintenance.name, WarehouseMaintenance);

Vue.use(VCharts);
Vue.use(ViewUI);
Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  components: { App },
  template: '<App/>'
})
