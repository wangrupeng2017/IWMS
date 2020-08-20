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
      setChooseWarehouse: "setChooseWarehouse"
    }),
    // 上一个仓库
    choosePrevious() {
      let warehouseIndex = this.chooseWarehouse - 1;
      if (warehouseIndex < 0) {
        warehouseIndex = this.warehouseList.length-1;
      }

      this.goChooseWarehouse(warehouseIndex);
    },
    // 下一个仓库
    chooseNext() {
      let warehouseIndex = this.chooseWarehouse + 1;
      if (warehouseIndex >= this.warehouseList.length) {
        warehouseIndex = 0;
      }

      this.goChooseWarehouse(warehouseIndex);
    },
    // 切换仓库
    goChooseWarehouse(warehouseIndex) {
      // 取出路由，跳转路由
      this.$router.push({
        name: this.warehouseList[warehouseIndex].router,
        params: { warehouseId: this.warehouseList[warehouseIndex].id }
      })
      // 更新store中仓库的选择
      this.setChooseWarehouse(warehouseIndex);
    },
  },
  computed: {
    ...mapState({
      chooseWarehouse: "chooseWarehouse",
      warehouseList: "warehouseList"
    })
  },
  mounted() {
    this.$router.push({
      name: this.warehouseList[this.chooseWarehouse].router,
      params: { warehouseId: this.warehouseList[this.chooseWarehouse].id }
    })
  }
}
</script>


<style lang="scss">
/** 文字溢出显示省略号 **/
@mixin TextOverflowEllipsis {
  overflow: hidden;
  text-overflow:ellipsis;
  white-space: nowrap;
}

// 仓库列表组件
#Warehouses-List {
  box-sizing: border-box;
  padding-top: 15px;
  padding-left: 100px;
  padding-right: 100px;
  position: relative;
  height: 220px;
  width: 100%;

  // 仓库列表 左/右 箭头
  .left-arrow, 
  .right-arrow {
    align-items: center;
    border-radius: 3px;
    cursor: pointer;
    display: flex;
    justify-content: center;
    opacity: 0;
    position: absolute;
    top: 50px;
    transition: all 0.3s;
    height: 120px;
    width: 60px;  
    .arrow-icon {
      font-size: 30px;
    }

    &:hover {
      background-color: #fff;
      box-shadow: 1px 1px 10px 2px #CCC;
      opacity: .8;
    }
  }
  .left-arrow {
    left: 20px;
  }
  .right-arrow {
    right: 20px;
  }

  // 仓库列表
  .warehouses-list {
    align-items: center;
    box-sizing: border-box;
    display: flex;
    flex-wrap: wrap;
    overflow: hidden;
    height: 100%;
    width: 100%;
  }

  // 仓库项
  .warehouses-item {
    align-items: center;
    background-color: #fff;
    border-radius: 8px;
    box-sizing: border-box;
    cursor: pointer;
    height: 195px;
    margin: 5px 10px 0;
    max-width: 180px;
    min-width: 180px;
    padding: 15px 15px 0;
    position: relative;
    transition: box-shadow .4s;

    // 仓库项卡片图片
    .warehouses-img-box {
      align-items: center;
      background-color: #f7f7f7;
      box-sizing: border-box;
      display: flex;
      height: 150px;
      width: 100%;
      .warehouses-img {
        width: 100%;
      }
    }

    // 仓库项卡片名称
    .warehouses-name {
      line-height: 100%;
      margin-top: 6px;
      text-align: center;
      @include TextOverflowEllipsis;
    }

    // 仓库项卡片阴影
    &:hover {
      box-shadow: 1px 1px 10px 2px #CCC;
    }
    &.is-checked {
      box-shadow: 1px 1px 10px 2px #CCC;
      font-weight: 550;
    }
  }
}
</style>
