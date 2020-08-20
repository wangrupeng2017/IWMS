import Vue from 'vue'
import Router from 'vue-router'
import WarehouseList from '@/components/WarehouseList'
import WarehouseNormal from "@/components/WarehouseNormal"
import WarehouseMaintenance from "@/components/WarehouseMaintenance"

// 这里是为了解决重复跳转相同路由会报错的情况
const originalPush = Router.prototype.push
Router.prototype.push = function push(location) {
  return originalPush.call(this, location).catch(err => err)
}

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/WarehouseNormal',
      name: 'WarehouseNormal',
      component: WarehouseNormal
    },
    {
      path: '/WarehouseMaintenance',
      name: 'WarehouseMaintenance',
      component: WarehouseMaintenance
    }
  ]
})
