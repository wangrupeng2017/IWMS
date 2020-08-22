<template>
  <div id="WarehouseNormal">
    <!-- 仓库实时 温度/湿度/光照 信息 -->
    <warehouses-info></warehouses-info>
    <!-- 仓库 统计/走势/设备控制 -->
    <history-statistics></history-statistics>
  </div>
</template>

<script>
export default {
  name: 'WarehouseNormal',
  mounted() {
    console.log(this.$route.params)
  }
}
</script>

<style lang="scss"
       src="../style/warehouse_normal.scss">
</style>