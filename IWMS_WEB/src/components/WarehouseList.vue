<template>
  <div id="Warehouses-List">
    <!-- 左箭头 -->
    <div class="left-arrow" @click="choosePrevious">
      <div class="arrow-icon iconfont iconjiantou_zuo"></div>
    </div>

    <!-- 仓库列表 -->
    <div class="warehouses-list">
      <!-- 仓库项卡片 -->
      <div class="warehouses-item" v-for="(warehouse,i) in warehouseList" :key="i" 
           :class="i==chooseWarehouse? 'is-checked' : ''"
           @click="goChooseWarehouse(i)">
        <div class="warehouses-img-box">
          <img class="warehouses-img" :src="warehouse.image" alt="house">
        </div>
        <div class="warehouses-name">{{warehouse.name}}</div>
      </div>
    </div>

    <!-- 右箭头 -->
    <div class="right-arrow" @click="chooseNext">
      <div class="arrow-icon iconfont iconjiantou_you"></div>
    </div>
  </div>
</template>

<script>
import {mapState, mapGetters, mapMutations, mapActions} from "vuex";

export default {
  name: 'WarehouseList',
  methods: {
    ...mapMutations({
      setChooseWarehouse: "setChooseWarehouse"  //设置 选择仓库的序号
    }),
    //  选择上一个仓库
    choosePrevious() {
      //  当前选择序号-1, 如果小于0就设置为最大可用序号
      let warehouseIndex = this.chooseWarehouse - 1;
      if (warehouseIndex < 0) {
        warehouseIndex = this.warehouseList.length-1;
      }
      //  显示所选仓库信息
      this.goChooseWarehouse(warehouseIndex);
    },
    //  选择下一个仓库
    chooseNext() {
      //  当前选择序号+1, 如果大于最大可用序号就设置为0
      let warehouseIndex = this.chooseWarehouse + 1;
      if (warehouseIndex >= this.warehouseList.length) {
        warehouseIndex = 0;
      }
      //  显示所选仓库信息
      this.goChooseWarehouse(warehouseIndex);
    },
    //  显示仓库信息
    goChooseWarehouse(warehouseIndex) {
      //  取出路由，跳转路由
      this.$router.push({
        name: this.warehouseList[warehouseIndex].router,
        params: { warehouseId: this.warehouseList[warehouseIndex].id }
      })
      //  更新store中仓库的选择
      this.setChooseWarehouse(warehouseIndex);
    },
  },
  computed: {
    ...mapState({
      chooseWarehouse: "chooseWarehouse",  //当前选择仓库序号
      warehouseList  : "warehouseList"     //仓库列表
    })
  },
  mounted() {
    //  页面加载自动显示第一个仓库信息
    this.$router.push({
      name: this.warehouseList[this.chooseWarehouse].router,
      params: { warehouseId: this.warehouseList[this.chooseWarehouse].id }
    })
  }
}
</script>


<style lang="scss"
       src="../style/warehouse_list.scss">
</style>
