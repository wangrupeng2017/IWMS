<template>
  <div id="Warehouses-Info">
    <!-- 温度卡片 -->
    <div class="temperature-card">
      <!-- 卡片信息 -->
      <div class="card-info">
        <!-- 卡片图标 -->
        <div class="card-icon-box">
          <i class="card-icon iconfont iconwendu"></i>
          <div class="card-icon-message">温度</div>
        </div>
        <!-- 卡片内容 -->
        <div class="card-data">{{realtimeTemperature.value}}℃</div>
        <!-- 报警图标 -->
        <div class="alarm-icon-box" v-show="realtimeTemperature.alarm"
             :class="realtimeTemperature.status!=0?'is-alarm':''">
          <i class="alarm-icon iconfont iconbj"></i>
        </div>
      </div>
      <!-- 卡片设置 -->
      <div class="card-setting">
        <div class="control-setting">
         <div class="setting-item">
            <div class="setting-item-title">监控报警</div>
            <i-switch :checked="realtimeTemperature.alarm"
                      @on-change="changeTemperatureAlarm"></i-switch>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">自动控制</div>
            <i-switch :checked="realtimeTemperature.automation"
                      @on-change="changeTemperatureAutomation"></i-switch>
          </div>
        </div>
        <div class="range-setting">
          <div class="setting-item">
            <div class="setting-item-title">最小值(℃)</div>
            <Input-number :max="60" :min="-20" :step="0.5" 
                          @on-change="goUpdateTemperature"
                          v-model="realtimeTemperature.min"></Input-number>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">最大值(℃)</div>
            <Input-number :max="60" :min="-20" :step="0.5" 
                          @on-change="goUpdateTemperature"
                          v-model="realtimeTemperature.max"></Input-number>
          </div>
        </div>
      </div>
    </div>
    <!-- 湿度卡片 -->
    <div class="humidity-card">
      <!-- 卡片信息 -->
      <div class="card-info">
        <!-- 卡片图标 -->
        <div class="card-icon-box">
          <i class="card-icon iconfont iconIOTtubiao_huabanfuben"></i>
          <div class="card-icon-message">湿度</div>
        </div>
        <!-- 卡片内容 -->
        <div class="card-data">{{realtimeHumidity.value}}％</div>
        <!-- 报警图标 -->
        <div class="alarm-icon-box" v-show="realtimeHumidity.alarm"
             :class="realtimeHumidity.status!=0?'is-alarm':''">
          <i class="alarm-icon iconfont iconbj"></i>
        </div>
      </div>
      <!-- 卡片设置 -->
      <div class="card-setting">
        <div class="control-setting">
         <div class="setting-item">
            <div class="setting-item-title">监控报警</div>
            <i-switch :checked="realtimeHumidity.alarm"
                      @on-change="changeHumidityAlarm"></i-switch>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">自动控制</div>
            <i-switch :checked="realtimeHumidity.automation"
                      @on-change="changeHumidityAutomation"></i-switch>
          </div>
        </div>
        <div class="range-setting">
          <div class="setting-item">
            <div class="setting-item-title">最小值(％)</div>
            <Input-number :max="100" :min="0" :step="0.5" 
                          @on-change="goUpdateHumidity"
                          v-model="realtimeHumidity.min"></Input-number>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">最大值(％)</div>
            <Input-number :max="100" :min="0" :step="0.5" 
                          @on-change="goUpdateHumidity"
                          v-model="realtimeHumidity.max"></Input-number>
          </div>
        </div>
      </div>
    </div>
    <!-- 光照度卡片 -->
    <div class="illuminance-card">
      <!-- 卡片信息 -->
      <div class="card-info">
        <!-- 卡片图标 -->
        <div class="card-icon-box">
          <i class="card-icon iconfont iconliangdu"></i>
          <div class="card-icon-message">光照</div>
        </div>
        <!-- 卡片内容 -->
        <div class="card-data">{{realtimeIlluminance.value}}％</div>
        <!-- 报警图标 -->
        <div class="alarm-icon-box" v-show="realtimeIlluminance.alarm"
             :class="realtimeIlluminance.status!=0?'is-alarm':''">
          <i class="alarm-icon iconfont iconbj"></i>
        </div>
      </div>
      <!-- 卡片设置 -->
      <div class="card-setting">
        <div class="control-setting">
         <div class="setting-item">
            <div class="setting-item-title">监控报警</div>
            <i-switch :checked="realtimeIlluminance.alarm"
                      @on-change="changeIlluminanceAlarm"></i-switch>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">自动控制</div>
            <i-switch :checked="realtimeIlluminance.automation"
                      @on-change="changeIlluminanceAutomation"></i-switch>
          </div>
        </div>
        <div class="range-setting">
          <div class="setting-item">
            <div class="setting-item-title">最小值(％)</div>
            <Input-number :max="100" :min="0" :step="0.5" 
                          @on-change="goUpdateIlluminance"
                          v-model="realtimeIlluminance.min"></Input-number>
          </div>
          <div class="setting-item">
            <div class="setting-item-title">最大值(％)</div>
            <Input-number :max="100" :min="0" :step="0.5" 
                          @on-change="goUpdateIlluminance"
                          v-model="realtimeIlluminance.max"></Input-number>
          </div>
        </div>
      </div>
    </div>

    <!-- 报警卡片 -->
    <div class="alarm-card" :class="alarmMessage!='全部正常'?'is-alarm':''">
      <div class="alarm-status">
        <i class="status-icon iconfont iconbj"></i>
        <div class="status-desc">{{alarmMessage}}</div>
      </div>
      <div class="alarm-status">
        <i class="status-icon iconfont iconhorn"></i>
        <div class="status-desc">{{deviceStatus.buzzer?"开启":"关闭"}}</div>
      </div>
      <div class="alarm-status">
        <i class="status-icon iconfont iconzhaoming-kai"></i>
        <div class="status-desc">{{deviceStatus.light?"开启":"关闭"}}</div>
      </div>
      <div class="alarm-status">
        <i class="status-icon iconfont iconfengkong"></i>
        <div class="status-desc">{{fanStatus}}</div>
      </div>

    </div>
  </div>
</template>

<script>
import {mapState, mapGetters, mapMutations, mapActions} from "vuex";

export default {
  name: 'WarehousesInfo',
  data() {
    return {
      // 循环请求定时器
      temperatureTimer: null,
      humidityTimer:    null,
      illuminanceTimer: null,
      deviceTimer:      null,
      // 延时消抖定时器
      temperatureTimer2: null,
      humidityTimer2:    null,
      illuminanceTimer2: null,
    }
  },
  methods: {
    ...mapActions({
      reqRealtimeTemperature: "reqRealtimeTemperature",  //请求 实时温度参数 方法
      reqRealtimeHumidity:    "reqRealtimeHumidity",     //请求 实时湿度参数 方法
      reqRealtimeIlluminance: "reqRealtimeIlluminance",  //请求 实时光照参数 方法
      reqDeviceStatus:        "reqDeviceStatus",         //请求 实时设备状态 方法
      reqSetTemperatureConfig:"reqSetTemperatureConfig", //请求 设置温度参数 方法
      reqSetHumidityConfig:   "reqSetHumidityConfig",    //请求 设置湿度参数 方法
      reqSetIlluminanceConfig:"reqSetIlluminanceConfig", //请求 设置光照参数 方法
    }),
    changeTemperatureAlarm(val)      { this.realtimeTemperature.alarm = val;      this.goUpdateTemperature(); },
    changeTemperatureAutomation(val) { this.realtimeTemperature.automation = val; this.goUpdateTemperature(); },
    changeHumidityAlarm(val)         { this.realtimeHumidity.alarm = val;         this.goUpdateHumidity();    },
    changeHumidityAutomation(val)    { this.realtimeHumidity.automation = val;    this.goUpdateHumidity();    },
    changeIlluminanceAlarm(val)      { this.realtimeIlluminance.alarm = val;      this.goUpdateIlluminance(); },
    changeIlluminanceAutomation(val) { this.realtimeIlluminance.automation = val; this.goUpdateIlluminance(); },
    // 发送请求 更新仓库温度设置
    goUpdateTemperature() {
      let timerLength = 500;  //500毫秒延时消抖
      if (this.temperatureTimer2) clearTimeout(this.temperatureTimer2);
      this.temperatureTimer2 = setTimeout(() => {
        this.reqSetTemperatureConfig();
        this.temperatureTimer2 = null;
      }, timerLength);
    },
    // 发送请求 更新仓库湿度设置
    goUpdateHumidity() {
      let timerLength = 500;  //500毫秒延时消抖
      if (this.humidityTimer2) clearTimeout(this.humidityTimer2);
      this.humidityTimer2 = setTimeout(() => {
        this.reqSetHumidityConfig();
        this.humidityTimer2 = null;
      }, timerLength);
    },
    // 发送请求 更新仓库光照设置
    goUpdateIlluminance() {
      let timerLength = 500;  //500毫秒延时消抖
      if (this.illuminanceTimer2) clearTimeout(this.illuminanceTimer2);
      this.illuminanceTimer2 = setTimeout(() => {
        this.reqSetIlluminanceConfig();
        this.illuminanceTimer2 = null;
      }, timerLength);
    },
  },
  computed: {
    ...mapState({
      realtimeTemperature: "realtimeTemperature",  //实时温度参数值
      realtimeHumidity:    "realtimeHumidity",     //实时湿度参数值
      realtimeIlluminance: "realtimeIlluminance",  //实时光照参数值
      deviceStatus:        "deviceStatus"          //实时设备状态值
    }),
    alarmMessage() {
      if (this.realtimeTemperature.status)      return "温度异常";
      else if (this.realtimeHumidity.status)    return "湿度异常";
      else if (this.realtimeIlluminance.status) return "光照异常";
      return "全部正常";
    },
    fanStatus() {
      switch(this.deviceStatus.fan) {
        case 0: return "关闭";
        case 1: return "1档";
        case 2: return "2档";
        case 3: return "3档";
      }
    }
  },
  mounted() {
    // 3秒循环请求
    let timerLength = 1000 * 10;
    // 开启 请求实时温度信息 定时器
    this.temperatureTimer = setInterval(this.reqRealtimeTemperature, timerLength);
    // 开启 请求实时湿度信息 定时器
    this.humidityTimer    = setInterval(this.reqRealtimeHumidity, timerLength);
    // 开启 请求实时光照信息 定时器
    this.illuminanceTimer = setInterval(this.reqRealtimeIlluminance, timerLength);
    // 开启 请求设备状态信息 定时器
    this.deviceTimer      = setInterval(this.reqDeviceStatus, timerLength);
  },
  beforeDestroy() {
    // 销毁 请求实时温度信息 定时器
    clearInterval(this.temperatureTimer);
    // 销毁 请求实时湿度信息 定时器
    clearInterval(this.humidityTimer);
    // 销毁 请求实时光照信息 定时器
    clearInterval(this.illuminanceTimer);
    // 销毁 请求设备状态信息 定时器
    clearInterval(this.deviceTimer);
  }
}
</script>

<style lang="scss">
// 警报动画
@keyframes myfirst
{
    0%   {color: #fff; }
    25%  {color: red;  }
    50%  {color: #fff; }
    75%  {color: red;  }
    100% {color: #fff; }
}
// 信息卡片颜色
@mixin CardInfoMixin($themeColor) {
  background-color: $themeColor;
  border-radius: 5px;
  box-sizing: border-box;
  box-shadow: 1px 1px 10px 2px #CCC;
  color: #fff;
  height: 100%;
  width: 400px;
  transition: all .1s;
  // 卡片信息
  .card-info {
    height: 200px;
    position: relative;
    width: 100%;
    // 卡片图标
    .card-icon-box {
      align-items: center;
      display: flex;
      height: 80px;
      left: 0;
      position: absolute;
      top: 0;
      width: 150px;
      .card-icon {
        margin-left: 10px;
        font-size: 60px;
      }
      .card-icon-message {
        font-size: 40px;
      }
    }
    // 卡片信息
    .card-data {
      align-items: flex-end;
      display: flex;
      font-weight: 550;
      font-size: 90px;
      justify-content: center;
      height: 95%;
    }
    // 报警图标
    .alarm-icon-box {
      align-items: center;
      display: flex;
      justify-content: center;
      height: 60px;
      position: absolute;
      right: 0;
      top: 0;
      width: 60px;
      
      .alarm-icon {
        font-size: 30px;
      }
      &.is-alarm {
        animation-name: myfirst;
        animation-duration: 2s;
        animation-iteration-count: infinite;
      }
    }
  }
  // 卡片设置
  .card-setting {
    box-sizing: border-box;
    padding: 15px 30px;
    height: 100px;
    width: 100%;
    
    .range-setting {
      display: flex;
      justify-content: space-between;
      .setting-item {
        align-items: center;
        display: flex;
        margin-bottom: 5px;
        width: 165px;
        .setting-item-title {
          font-weight: 550;
          margin-right: 5px;
          width: 78px;
        }
      }
    }
    .control-setting {
      display: flex;
      justify-content: space-between;
      .setting-item {
        align-items: center;
        display: flex;
        height: 30px;
        margin-bottom: 5px;
        width: 165px;
        .setting-item-title {
          font-weight: 550;
          margin-right: 5px;
          width: 78px;
        }
      }
    }
  }

  &:hover {
    background-color: mix(#fff,$themeColor, 10%);
  }
}

#Warehouses-Info {
  align-items: center;
  box-sizing: border-box;
  display: flex;
  justify-content: space-between;
  height: 300px;
  margin-top: 15px;
  padding: 0 100px;
  width: 100%;

  .temperature-card {
    @include CardInfoMixin(#db6623);
  }
  .humidity-card {
    @include CardInfoMixin(#5271C2);
  }
  .illuminance-card {
    @include CardInfoMixin(#aa9e5c);
  }
  .alarm-card {
    background-color: #00cc66;
    border-radius: 5px;
    box-sizing: border-box;
    box-shadow: 1px 1px 10px 2px #CCC;
    color: #fff;
    height: 100%;
    padding: 10px 20px;
    width: 300px;
    min-width: 300px;
    transition: all .1s;
    .alarm-status {
      align-items: center;
      display: flex;
      height: 60px;
      width: 100%;
      .status-icon {
        font-size: 40px;
      }
      .status-desc {
        flex: 1;
        font-size: 30px;
        margin-left: 15px;
      }
      &:nth-child(1) {
        margin-bottom: 25px;
      }
    }
    &:hover {
      background-color: mix(#fff,#00cc66, 10%);
    }
    &.is-alarm {
      background-color: #ff6600;
    }
  }
}
</style>