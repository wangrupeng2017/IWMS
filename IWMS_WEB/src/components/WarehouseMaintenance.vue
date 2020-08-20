<template>
  <div id="WarehouseMaintenance">
    <div class="maintenance-title">仓库正在建设中...</div>
    <div class="maintenance-img-box">
      <img class="maintenance-img" src="/static/house1.png" alt="">
    </div>
  </div>
</template>

<script>
export default {
  name: "WarehouseMaintenance"
}
</script>

<style lang="scss">
#WarehouseMaintenance {
  height: 700px;
  width: 100%;

  .maintenance-title {
    font-size: 60px;
    font-weight: 550;
    margin-top: 50px;
    text-align: center;
    width: 100%;
  }

  .maintenance-img-box {
    display: flex;
    justify-content: center;
    margin-top: 50px;
    width: 100%;
    .maintenance-img {
      width: 500px;
    }
  }

}
</style>