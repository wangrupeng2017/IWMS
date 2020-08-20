<template>
  <div id="WarehouseNormal">
    <warehouses-info></warehouses-info>
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

<style lang="scss">
#WarehouseNormal {

}
</style>